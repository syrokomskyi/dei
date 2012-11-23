#include "../include/StdAfx.h"
#include "../include/Delivery.h"
//#include "../external/cajun/json/reader.h"
//#include "../external/cajun/json/writer.h"
//#include "../external/cajun/json/elements.h"
#include <typelib/typelib.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>



std::string dei::Delivery::typeDelivery() const {

    // в комментрии встретилась "дверь" - в *нашем случае* этого достаточно,
    // чтобы однозначно определить тип доставки
    const std::string lowerNote = boost::to_lower_copy( note, std::locale( "" ) );
    const bool hasDoor = boost::contains( lowerNote, "двер" );

    return hasDoor ? "stock-door" : "stock-stock";
}






void dei::Delivery::load(
    listDeliveryPtr_t& r,
    const std::string& source,
    const std::string& configure
) {

    std::ifstream in( source.c_str() );
    if ( !in.is_open() ) {
        throw Exception( "Файл \"" + source + "\" не найден." );
    }

    // @source http://mybyteofcode.blogspot.com/2010/11/parse-csv-file-with-embedded-new-lines.html
    typedef boost::tokenizer< boost::escaped_list_separator< char > >  Tokenizer;

    const boost::escaped_list_separator< char > sep( '\\', ';', '\"' );

    std::vector< std::string > vec;
    std::string line;
    std::string buffer;

    bool inside_quotes( false );
    size_t last_quote( 0 );

    // контроль: столбцов должно быть не более 11-ти
    bool above11 = false;

    // номер тек. строки
    size_t i = 0;
    while ( std::getline( in, buffer ) ) {
        ++i;
        // первая строка - пропускаем: содержит имена столбцов
        if (i == 1) {
            continue;
        }

        // одиночная обратная косая может натворить бед
        boost::replace_all( buffer, "\\", " " );

        // --- deal with line breaks in quoted strings
        last_quote = buffer.find_first_of( '"' );
        while (last_quote != std::string::npos) {
            inside_quotes = !inside_quotes;
            last_quote = buffer.find_first_of( '"', last_quote + 1 );
        }

        line.append( buffer );

        if ( inside_quotes ) {
            line.append( "\n" );
            continue;
        }

        Tokenizer tok( line, sep );
        vec.assign( tok.begin(), tok.end() );

        // clear here, next check could fail
        line.clear();

        // Проверяем кол-во полей в распарсенной строке
        // @see data/1.csv
        if (vec.size() < 11) {
            std::ostringstream ss;
            ss << vec.size();
            throw Exception(
                static_cast< std::string>( "Кол-во строк в файле-источнике не совпадает с ожидаемым." )
              + " Получено: " + ss.str() + "."
              + " Ожидается: 11."
              + " Если файл сохранялся из Excel, возможно, добавлялись новые столбцы. Удалите их и сформируйте CSV-файл заново."
            );
        }
        if (vec.size() > 11) {
            above11 = true;
        }

        // создаём доставку
        // доставка создаётся со значениями по умолчанию
        DeliveryPtr d( new Delivery( configure ) );
        d->date = correctField( vec.at( 0 ) );
        d->number = correctField( vec.at( 1 ) );
        d->item = correctField( vec.at( 2 ) );
        d->name = correctField( vec.at( 3 ) );
        d->phone = correctField( vec.at( 4 ) );
        d->postcode = correctField( vec.at( 5 ) );

        // город проходит спец. очистку
        d->city = correctCity( vec.at( 6 ) );

        d->address = correctField( vec.at( 7 ) );
        d->total = correctField( vec.at( 8 ) );
        d->totalClear = correctField( vec.at( 9 ) );
        d->note = correctField( vec.at( 10 ) );


        // формируем результат
        r.push_back( d );

        // @test 1 заявка
        //break;

        //std::copy( vec.begin(),  vec.end(),  std::ostream_iterator< std::string >( std::cout, "|") );
        //std::cout << "\n----------------------" << std::endl;

    } // while ( std::getline( in, buffer ) )

    in.close();

    if ( above11 ) {
        std::ostringstream ss;
        ss << vec.size();
        std::cout << "(?) В файле CSV обнаружено больше 11-ти столбцов. Столбцы 12 (L) и далее (M, N, ...) проигнорируются." << std::endl;
    }


}







void dei::Delivery::initDefault(
    dei::Delivery* d, const std::string&  initFile
) {

    // Инициируем некоторые значения полей по умолчанию (на случай, если в
    // файл конфигурации они не будут включены)
    d->ignoreItem = false;


    // получаем json-объект из файла
    std::ifstream in( initFile.c_str() );
    if ( !in.is_open() ) {
        throw dei::Exception( "Файл \"" + initFile + "\" не найден." );
    }
    std::string tempStr;
    std::string str = "";
    while( getline( in, tempStr ) ) {
        str += tempStr;
    }
    in.close();


    // @todo fine Использовать парсер из typelib.

    // @see external/cajun/test.cpp
    std::istringstream iss( str );
    ::json::Object obj;
    try {
        ::json::Reader::Read( obj, iss );

    } catch ( const ::json::Reader::ParseException& e ) {
        std::ostringstream ss;
        ss << "Ошибка в структуре файла \"" << initFile << "\".  "
            << e.what()
            << " Line / offset: " << (e.m_locTokenBegin.m_nLine + 1)
            << '/' << (e.m_locTokenBegin.m_nLineOffset + 1);
        throw Exception( ss.str() );
    }

    // Проанализируем результат, и, чтобы не нарушать код сторонней библиотеки:
    //   - заменим "\u" (так уж работает Cajun) на 'u'
    for (auto itr = obj.Begin(); itr != obj.End(); ++itr) {
        const auto o = *itr;

        /* Структура файла:
            "novaposhta": {
              "source": "1.csv",
              "oauth": "000",
              "init-command": {
                "create-ttn": {
                  "order_id": "1",
                  "date": "2",
                  "date_desired": "3",
                  ...
        */

        /* - @todo fine Не работает. Причина?
        // пропускаем примечания
        const bool commentField = (o.name.find( "//" ) == 0);
        if ( commentField ) {
            continue;
        }
        */

        if (o.name == "novaposhta") {
            const auto oo = static_cast< json::Object >( o.element );
            for (auto itr = oo.Begin(); itr != oo.End(); ++itr) {
                const auto o = *itr;

                if (o.name == "oauth") {
                    d->oauth = static_cast< json::String >( o.element ).Value();
                    continue;
                }

                if (o.name == "ignore-item") {
                    d->ignoreItem = static_cast< json::Boolean >( o.element ).Value();
                    continue;
                }

                if (o.name == "scream-door-note") {
                    d->screamDoorNote = static_cast< json::String >( o.element ).Value();
                    continue;
                }

                if (o.name == "init-command") {
                    const auto oo = static_cast< json::Object >( o.element );
                    for (auto itr = oo.Begin(); itr != oo.End(); ++itr) {
                        const auto o = *itr;

                        if (o.name == "create-ttn") {
                            const auto oo = static_cast< json::Object >( o.element );
                            for (auto itr = oo.Begin(); itr != oo.End(); ++itr) {
                                const auto o = *itr;

                                // @todo fine Избавится от лапши, введя класс Key
                                // для всех json-подобных объектов.
                                if (o.name == "sender_city") {
                                    d->senderCity = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "sender_company") {
                                    d->senderCompany = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "sender_address") {
                                    d->senderAddress = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "sender_contact") {
                                    d->senderContact = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "sender_phone") {
                                    d->senderPhone = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "rcpt_warehouse") {
                                    d->warehouse = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "pay_type") {
                                    d->payType = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "payer") {
                                    d->payer = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "cost") {
                                    d->declareCost = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "additional_info") {
                                    d->additionalInfo = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "pack_type") {
                                    d->packType = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "description") {
                                    d->description = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "delivery_amount") {
                                    d->deliveryAmount = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "redelivery_type") {
                                    d->redeliveryType = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "delivery_in_out") {
                                    d->deliveryInOut = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "redelivery_payment_city") {
                                    d->redeliveryPaymentCity = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "redelivery_payment_payer") {
                                    d->redeliveryPaymentPayer = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "weight") {
                                    d->weight = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "length") {
                                    d->length = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "width") {
                                    d->width = static_cast< json::String >( o.element ).Value();
                                } else if (o.name == "height") {
                                    d->height = static_cast< json::String >( o.element ).Value();

                                } else if (o.name == "cont_description") {
                                    d->containerDescription = static_cast< json::String >( o.element ).Value();

                                } else {
                                    // не все элементы нам нужны для иниуиализации
                                }

                            } // for (auto itr = oo.Begin(); itr != oo.End(); ++itr)

                        } // if (o.name == "create-ttn")

                    } // for (auto itr = ...

                } // if (o.name == "init-command")


                // # В конфигурации "novaposhta" не должно быть полей разных
                //   типа, заканчивающихся на "-delivery".
                const bool deliveryField = boost::ends_with( o.name, "-delivery" );
                if ( deliveryField && !o.name.empty() && (o.name[0] != '/') ) {
                    const auto l = static_cast< json::Array >( o.element );
                    if ( l.Size() != 2 ) {
                        std::ostringstream ss;
                        ss << "В файле \"" << initFile << "\" для поля \"" << o.name << "\""
                           << " надо указать 2 параметра.";
                        throw Exception( ss.str() );
                    }

                    if (o.name == "door-door-delivery") {
                        d->doorDoorDelivery.push_back( static_cast< json::String >( l[0] ).Value() );
                        d->doorDoorDelivery.push_back( static_cast< json::String >( l[1] ).Value() );

                    } else if (o.name == "door-stock-delivery") {
                        d->doorStockDelivery.push_back( static_cast< json::String >( l[0] ).Value() );
                        d->doorStockDelivery.push_back( static_cast< json::String >( l[1] ).Value() );

                    } else if (o.name == "stock-door-delivery") {
                        d->stockDoorDelivery.push_back( static_cast< json::String >( l[0] ).Value() );
                        d->stockDoorDelivery.push_back( static_cast< json::String >( l[1] ).Value() );

                    } else if (o.name == "stock-stock-delivery") {
                        d->stockStockDelivery.push_back( static_cast< json::String >( l[0] ).Value() );
                        d->stockStockDelivery.push_back( static_cast< json::String >( l[1] ).Value() );
                    }

                }

            } // for (auto itr = ...


        } // if (o.name == "novaposhta")



        // @todo fine Нет, спрятать сложность парсера в typelib.
        break;

    } // for (auto itr = obj.Begin(); itr != obj.End(); ++itr)


}







std::string dei::Delivery::correctField( const std::string& s ) {
    return boost::trim_copy( s, std::locale( "" ) );
}






std::string dei::Delivery::correctCity( const std::string& s ) {

    std::string r = boost::trim_copy( s, std::locale( "" ) );

    // удаляем человеческие сочетания
    if ( (r[0] == 'г') || (r[0] == 'Г') ) {
        boost::erase_first( r, "г." );
        boost::erase_first( r, "Г." );
        boost::erase_first( r, "г " );
        boost::erase_first( r, "Г " );
    }

    boost::trim( r, std::locale( "" ) );

    return r;
}







std::ostream& std::operator<<(
    std::ostream& out,
    const dei::listDeliveryPtr_t& ld
) {
    out << "{ \"Delivery list\": ";
    typelib::printPtr( out, ld );
    out << " }";

    return out;
}





std::ostream& std::operator<<(
    std::ostream& out,
    const dei::Delivery& d
) {
    out << "{ \"Delivery\": " << d.number << " }" << std::endl;
    // @todo Печатать содержание.

    return out;
}
