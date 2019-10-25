//============================================================================
// @author  : sunquan
// @date   : 2019-09-25 18:44:22
//============================================================================
#pragma once
#include <iostream>
#include <ql/quantlib.hpp>
using namespace QuantLib;

namespace TestDate {

Real CalcReciprocal(Real x)
{
    QL_REQUIRE(x != 0, "Divided by Zero");
    return 1 / x;
}

/**
 * 测试 date 的相关API
 */
void Test_DateAPI()
{
    Date date(18, September, 2019);
    std::cout << "-------- Test_DateAPI ---------->" << std::endl;
    std::cout << "[date]:" << date << std::endl;
    date++;
    std::cout << date << std::endl;
    date += 12 * Days;
    std::cout << "[after 2 days]: " << date << std::endl;
    date -= 2 * Months;
    std::cout << "[before 2 months]: " << date << std::endl;
    date--;
    std::cout << "[date--]: " << date << std::endl;

    Period myP(10, Weeks);
    date += myP;
    std::cout << "[after 10 weeks]: " << date << std::endl << std::endl;
}

void DateTesting2()
{
    Date date(18, September, 2019);
    std::cout << "Original Date:" << date << std::endl;
    std::cout << "Weekday:" << date.weekday() << std::endl;
    std::cout << "Day of Month:" << date.dayOfMonth() << std::endl;
    std::cout << "Day of Year:" << date.dayOfYear() << std::endl;
    std::cout << "Month:" << date.month() << std::endl;
    int month_int = date.month();
    std::cout << "Month via Integer:" << month_int << std::endl;
    std::cout << "Year:" << date.year() << std::endl;
    int serialNum = date.serialNumber();
    std::cout << "Serial Number:" << serialNum << std::endl << std::endl;
}

void DateTesting3()
{
    std::cout << "Today:" << Date::todaysDate() << std::endl;
    std::cout << "Min Date:" << Date::minDate() << std::endl;
    std::cout << "Max Date:" << Date::maxDate() << std::endl;
    std::cout << "Is Leap:" << Date::isLeap(2019) << std::endl;
    std::cout << "End of Month:" << Date::endOfMonth(Date(4, Aug, 2019)) << std::endl;
    std::cout << "Is Month End:" << Date::isEndOfMonth(Date(29, Sep, 2019)) << std::endl;
    std::cout << "Is Month End:" << Date::isEndOfMonth(Date(30, Sep, 2019)) << std::endl;
    std::cout << "Next WD:" << Date::nextWeekday(Date(1, Sep, 2019), Friday) << std::endl;
    std::cout << "n-th WD:" << Date::nthWeekday(3, Wed, Sep, 2019) << std::endl << std::endl;
}

void DateTesting4()
{
    Date d = Settings::instance().evaluationDate();
    std::cout << "Eval Date:" << d << std::endl;
    Settings::instance().evaluationDate() = Date(5, Jan, 1995);
    d = Settings::instance().evaluationDate();
    std::cout << "New Eval Date:" << d << std::endl;
}

void CalendarTesting1()
{
    Calendar frankfCal = Germany(Germany::FrankfurtStockExchange);
    Calendar saudiArabCal = SaudiArabia();
    Date nyEve(31, Dec, 2009);
    std::cout << "Is BD:" << frankfCal.isBusinessDay(nyEve) << std::endl;
    std::cout << "Is Holiday:" << frankfCal.isHoliday(nyEve) << std::endl;
    std::cout << "Is Weekend:" << saudiArabCal.isWeekend(Saturday) << std::endl;
    std::cout << "Is Last BD:" << frankfCal.isEndOfMonth(Date(30, Dec, 2009)) << std::endl;
    std::cout << "Last BD:" << frankfCal.endOfMonth(nyEve) << std::endl;
}

void run()
{
    std::cout << "Current QL Version: " << QL_LIB_VERSION << std::endl;
    std::cout << "计算倒数：" << CalcReciprocal(2.0) << std::endl;

    Test_DateAPI();
    DateTesting2();
    DateTesting3();
    DateTesting4();

    CalendarTesting1();
}

}
