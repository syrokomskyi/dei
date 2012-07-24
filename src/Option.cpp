#include "../include/StdAfx.h"
#include "../include/Option.h"


dei::Option* dei::Option::instance = nullptr;



dei::Option::Option() {

    // @todo ...

}







std::ostream& std::operator<<(
    std::ostream& out,
    const dei::Option& o
) {
    out << "\"Option\"";
    return out;
}
