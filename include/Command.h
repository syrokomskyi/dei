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
    * Файл конфигурации.
    */
    const std::string configure;


    /**
    * Входящие ТТН (готовы к отправке).
    */
    listDeliveryPtr_t ld;

    
    //inline DeliveryCommand() : configure( "" ) {
    //}


    inline DeliveryCommand(
        const listDeliveryPtr_t& ld,
        const std::string& configure
    ) :
        configure( configure ),
        ld( ld )
    {
    }

};






/**
* Команды для работы с Новой почтой.
*/
namespace novaposhta {


/**
* Создание файла с готовой для импорта информацией о ТТН.
* Используется указанная конфигурация.
*/
struct CreateTTN : public DeliveryCommand {

    inline CreateTTN(
        const std::string& configure
    ) : DeliveryCommand( listDeliveryPtr_t(), configure ) {
    }

    inline CreateTTN(
        const listDeliveryPtr_t& ld,
        const std::string& configure
    ) : DeliveryCommand( ld, configure ) {
    }


    virtual void run();


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



protected:
    result_t mResult;

};






/**
* Создание файла с готовой для импорта информацией о ТТН.
* Используется конфигурация из "1-configure.json".
*/
struct CreateTTN1 : public CreateTTN {

    inline CreateTTN1(
    ) : CreateTTN( "1-configure.json" ) {
    }

    inline CreateTTN1(
        const listDeliveryPtr_t& ld
    ) : CreateTTN( ld, "1-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "Создать файл для импорта ТТН (наличные, платит получатель).";
    }


    virtual inline std::string hotkey() const {
        return "1";
    }


    virtual inline std::string confirmation() const {
        //@todo return "Оформить ТТН на сайте \"Новой почты\"? Данные будут взяты из файла."; - Объединить команды.
        std::ostringstream ss;
        ss << "Подготовить файл с информацией для импорта на сайте \"Новой почты\""
            << " (наличные, платит получатель)."
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
           << " Данные для импорта ТТН (наличные) доступны в файле \"data/b.xml\"."
           << " Откройте его и скопируйте в форму на сайте \"Новой почты\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "Файл для импорта ТТН (наличные) не создан.";
    }

};






/**
* Создание файла с готовой для импорта информацией о ТТН.
* Используется конфигурация из "2-configure.json".
*/
struct CreateTTN2 : public CreateTTN {

    inline CreateTTN2(
    ) : CreateTTN( "2-configure.json" ) {
    }

    inline CreateTTN2(
        const listDeliveryPtr_t& ld
    ) : CreateTTN( ld, "2-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "Создать файл для импорта ТТН (безналичные, платит отправитель).";
    }


    virtual inline std::string hotkey() const {
        return "2";
    }


    virtual inline std::string confirmation() const {
        //@todo return "Оформить ТТН на сайте \"Новой почты\"? Данные будут взяты из файла."; - Объединить команды.
        std::ostringstream ss;
        ss << "Подготовить файл с информацией для импорта на сайте \"Новой почты\""
            << " (безналичные, платит отправитель)."
            << " Данные будут взяты из файла \"1.csv\""
            << " и записаны в файл \"b.xml\". Откройте последний файл, чтобы быстро импортировать"
            << " заявки в личном кабинете \"Новой почты\".";
        return ss.str();
    }


    virtual inline std::string success() const {
        std::ostringstream ss;
        ss << "Получено заявок - " << ld.size() << "."
           << " Данные для импорта ТТН (безналичные) доступны в файле \"data/b.xml\"."
           << " Откройте его и скопируйте в форму на сайте \"Новой почты\".";
        return ss.str();
    }

    
    virtual inline std::string failure() const {
        return "Файл для импорта ТТН (безналичные) не создан.";
    }

};






/**
* Слияние файла с информацией о заявках и файла с номерами заявок
* от "Новой почты".
* Используется указанная конфигурация.
*/
struct FusionTTN : public DeliveryCommand {

    inline FusionTTN(
        const std::string& configure
    ) : DeliveryCommand( listDeliveryPtr_t(), configure ) {
    }


    virtual void run();


    friend Delivery;

};




/**
* Слияние файла с информацией о заявках и файла с номерами заявок
* от "Новой почты".
* Используется конфигурация из "1-configure.json".
*/
struct FusionTTN1 : public FusionTTN {

    // список заявок и конфигурация здесь не используются
    inline FusionTTN1(
    ) : FusionTTN( "1-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "Создать файл для быстрой связи заявок (наличные, платит получатель).";
    }


    virtual inline std::string hotkey() const {
        return "3";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "Конфигурация: наличные, платит получатель."
            << "\nИз подготовленного на сайте \"Новой почты\" файла \"b.xml\""
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

};




/**
* Слияние файла с информацией о заявках и файла с номерами заявок
* от "Новой почты".
* Используется конфигурация из "2-configure.json".
*/
struct FusionTTN2 : public FusionTTN {

    inline FusionTTN2(
    ) : FusionTTN( "2-configure.json" ) {
    }


    virtual inline std::string name() const {
        return "Создать файл для быстрой связи заявок (безналичные, платит отправитель).";
    }


    virtual inline std::string hotkey() const {
        return "4";
    }


    virtual inline std::string confirmation() const {
        std::ostringstream ss;
        ss << "Конфигурация: безналичные, платит отправитель."
            << "\nИз подготовленного на сайте \"Новой почты\" файла \"b.xml\""
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