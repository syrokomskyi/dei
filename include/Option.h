#pragma once

#include "configure.h"
#include <ostream>


namespace dei {

class Option;

}




namespace std {

ostream& operator<<( std::ostream& out, const dei::Option& );

} // std





namespace dei {


/**
* @todo Настройки из файла configure.json.
*/
class Option {

protected:
    /**
    * Загружает данные из файла dei::CONFIGURE.
    * Инициализирует объект.
    */
    Option();


public:
    static inline Option* singleton() {
        if ( instance ) {
            return instance;
        }
        instance = new Option();
        return instance;
    }



    virtual inline ~Option() {
    }



protected:
    /**
    * Объект JSON, представляющий собой набор значений по умолчанию и
    * настроек для проекта.
    */



    /**
    * Синглетон.
    */
    static Option* instance;

};



} // dei
