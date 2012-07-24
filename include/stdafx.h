#pragma once

// Длина внутреннего имени объекта в компиляторе превысила максимальное значение, имя усечено
#pragma warning( disable : 4503 )

// Компилятор обнаружил функцию, которая была помечена как устаревшая — deprecated
// @see http://lists.boost.org/Archives/boost/2006/08/109338.php 
#pragma warning( disable : 4996 )


// (!) Следующие предупреждения следует включать только если исп. чужие
//     библиотеки и они эти предупреждения выбрасывают.

// Виртуальная функция переопределяет "pcl::search::Search<PointT>::radiusSearch",
// предыдущие версии компилятора не выполняли переопределение, когда параметры
// отличались только квалификаторами const или volatile
//#pragma warning( disable : 4373 )

// Преобразование double в int, возможна потеря данных
//#pragma warning( disable : 4244 )



// Отключим вредный макрос в WinDef.h
#define NOMINMAX

// Используем математические константы
#define _USE_MATH_DEFINES


#include <assert.h>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <typeinfo>
#include <vector>
#include <windows.h>


#include <boost/lexical_cast.hpp>
#include <boost/numeric/interval.hpp>

#include "Exception.h"
