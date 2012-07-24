#pragma once

#include "Delivery.h"
#include <typelib/typelib.h>


namespace dei {
    namespace command {

struct Command;
typedef std::shared_ptr< Command >  CommandPtr;
typedef std::unique_ptr< Command >  CommandUPtr;
typedef std::vector< CommandPtr >  listCommandPtr_t;

    }
}



namespace dei {

    
/**
* Команды для работы с программой.
*/

namespace command {


    
struct Command {
    virtual void run() = 0;
    virtual std::string name() const = 0;
    virtual std::string hotkey() const = 0;
    virtual std::string confirmation() const = 0;
    virtual std::string success() const = 0;
    virtual std::string failure() const = 0;


protected:

    // для отправки запросов с помощью CURL
    static inline int writer( char *data, size_t size, size_t nmemb, std::string* dest ) {
        int written = 0;
        if ( dest ) {
            written = size * nmemb;
            dest->append( data, written );
        }
        return written;
    }


    // для получения ответов с помощью CURL
    static inline size_t reader( void* ptr, size_t size, size_t nmemb, std::string* stream ) {
        int actual  = static_cast< int >( stream->size() );
        int written = size * nmemb;
        if (written > actual) {
            written = actual;
        }
        memcpy( ptr, stream->c_str(), written );
        stream->erase( 0, written );
        return written;
    }

};





struct DeliveryCommand : public Command {
    /**
    * Входящие ТТН (готовы к отправке).
    */
    listDeliveryPtr_t ld;

    
    inline DeliveryCommand() {
    }


    inline DeliveryCommand( const listDeliveryPtr_t& ld ) : ld( ld ) {
    }

};






/**
* Команды для работы с Новой почтой.
*/
namespace novaposhta {


/**
* Создание файла с готовой для импорта информацией о ТТН.
*/
struct CreateTTN : public DeliveryCommand {

    inline CreateTTN() {
    }

    inline CreateTTN( const listDeliveryPtr_t& ld ) : DeliveryCommand( ld ) {
    }


    virtual void run();


    virtual inline std::string name() const {
        return "Создать файл для импорта ТТН.";
    }


    virtual inline std::string hotkey() const {
        return "1";
    }


    virtual inline std::string confirmation() const {
        //@todo return "Оформить ТТН на сайте \"Новой почты\"? Данные будут взяты из файла."; - Объединить команды.
        std::ostringstream ss;
        ss << "Подготовить файл с информацией для импорта на сайте \"Новой почты\"."
            << " Данные будут взяты из файла \"1.csv\""
            << " и записаны в файл \"b.xml\". Откройте последний файл, чтобы быстро импортировать"
            << " заявки в личном кабинете \"Новой почты\".";
        return ss.str();
    }


    virtual inline std::string success() const {
        /*
        std::ostringstream ss;
        ss << ld.size() << " ТТН отправлено."
           << " \"Новая почта\" присвоила накладным номера:" << std::endl;
        typelib::print( ss, mResult );
        return ss.str();
        */
        //return "Данные для импорта ТТН доступны в файле \"data/b.xml\". Откройте его и скопируйте в форму на сайте \"Новой почты\".";
        std::ostringstream ss;
        ss << "Получено заявок - " << ld.size() << "."
           << " Данные для импорта ТТН доступны в файле \"data/b.xml\"."
           << " Откройте его и скопируйте в форму на сайте \"Новой почты\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "Файл для импорта ТТН не создан.";
    }

    
    /**
    * Результат оформления ТТН.
    */
    struct Result {
        DeliveryPtr d;
        // Номер накладной в базе Новой почты.
        std::string uid;
    };
    typedef std::vector< Result >  result_t;
    
    inline result_t const& result() const {
        return mResult;
    }



private:
    result_t mResult;

};






/**
* Слияние файла с информацией о заявках и файла с номерами заявок
* от "Новой почты".
*/
struct FusionTTN : public DeliveryCommand {

    inline FusionTTN() {
    }


    virtual void run();


    virtual inline std::string name() const {
        return "Создать файл для быстрой связи заявок \"Новой почты\" с заявками Системы.";
    }


    virtual inline std::string hotkey() const {
        return "2";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "Из подготовленного на сайте \"Новой почты\" файла \"b.xml\""
            << " и файла \"1.csv\" создаётся \"2.csv\" для импорта в Систему."
            << " Импортируйте \"2.csv\" в Систему, чтобы быстро связать номера заявок"
            << " \"Новой почты\" с заявками в Системе.";
        return ss.str();
    }


    virtual inline std::string success() const {
        return "Все заявки оформлены.";
    }

    
    virtual inline std::string failure() const {
        return "Есть неоформленные заявки.";
    }



    friend Delivery;

};



} // novaposhta





} // command

} // dei






namespace std {


inline std::ostream& operator<<( std::ostream& out, const dei::command::Command& c ) {
    out << c.hotkey() << " " << c.name();

    return out;
}




/**
* Формирует запрос для отправки на сервер.
*/
std::ostream& operator<<( std::ostream& out, const dei::command::novaposhta::CreateTTN& );




inline std::ostream& operator<<( std::ostream& out, const dei::command::novaposhta::CreateTTN::Result& r ) {
    out << r.d->number << " -> " << r.uid << std::endl;

    return out;
}


} // std