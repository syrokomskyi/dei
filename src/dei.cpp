#include "../include/StdAfx.h"
#include "../include/configure.h"
#include "../include/Delivery.h"
#include "../include/Command.h"


/**
* Испорт из CSV (уже получен исходный файл).
* -> Передача данных Новой почте.
* -> Получение ответа.
* -> Экспорт в CSV.
*/
int main( int argc, char** argv ) {

    using namespace dei;
    namespace dc = dei::command;
    namespace dcn = dei::command::novaposhta;
    

    setlocale( LC_ALL, "Russian" );
    // Для разделителя '.' вместо ','
    setlocale( LC_NUMERIC, "C" );


    // Меню
    dc::listCommandPtr_t menu;
    menu.push_back( dc::CommandPtr( new command::novaposhta::CreateTTN1() ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::CreateTTN2() ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::FusionTTN1()  ) );
    menu.push_back( dc::CommandPtr( new command::novaposhta::FusionTTN2()  ) );

    static size_t rerun = 0;
    char ch = 0;
    while (ch != 13) {

        std::cout << "Что мне сделать" << ( (rerun > 0) ? " сейчас?" : "?") << std::endl << std::endl;
        rerun++;
        typelib::printPtr( std::cout, menu, "", "", "", "\n" );
        std::cout << std::endl << std::endl << "> ";
        //ch = std::getchar();
        //std::cout << ch << std::endl;
        std::cin >> ch;

        // Распознаём команду
        const std::string skey( 1, ch );
        auto itr = menu.begin();
        for ( ; itr != menu.end(); ++itr) {
            const dc::CommandPtr cmd = *itr;
            auto sk = cmd->hotkey();
            if (sk == skey) {
                try {
                    cmd->run();
                } catch ( const Exception& ex ) {
                    std::cout << "Ошибка" << std::endl;
                    std::cout << std::endl << "(!) " << ex.what() << std::endl;
                }
                break;
            }
        }

        // Завершаем работу при вводе '0' или написании 'exit', 'quit'
        if ( (ch == '0')
            || (ch == 'e') || (ch == 'q') || (ch == 'E') || (ch == 'Q')
            || (ch == '-') || (ch == '.') || (ch == ',')
        ) {
            break;
        }

        if (itr == menu.cend()) {
            std::cerr << std::endl << ch << "? Команда не известна." << std::endl;
        }

        // Распознанная команда выполнена выше

        std::cout << std::endl << std::endl << std::endl;

    } // while ...



    std::cout << std::endl << "Работа завершена." << std::endl;
    std::cout << std::endl << "Если что-то работает не так или хочется лучше > \nАндрей Сырокомский, +38 050 335-16-18, dmotion@yandex.ru" << std::endl;
    std::cout << "^" << std::endl << std::endl;
    //std::cin.ignore();
    std::system( "pause.exe" );

    return 0;

} // main()
