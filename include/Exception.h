#pragma once

#include <stdexcept>


namespace dei {

class Exception;
typedef std::shared_ptr< Exception >  ExceptionPtr;
typedef std::unique_ptr< Exception >  ExceptionUPtr;
typedef std::vector< dei::ExceptionPtr >  listExceptionPtr_t;

}




namespace dei {


/**
* Исключения для проекта.
*/
class Exception : public std::runtime_error {
public:
    inline Exception( const std::string& message ) : runtime_error( message ) {
    }


    virtual inline ~Exception() {
    }

};



} // dei
