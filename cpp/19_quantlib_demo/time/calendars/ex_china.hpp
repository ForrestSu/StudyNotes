/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*! \file ex_china.hpp
    \brief extend Chinese calendar
*/

#ifndef quantlib_ex_chinese_calendar_hpp
#define quantlib_ex_chinese_calendar_hpp

#include <ql/time/calendar.hpp>

namespace QuantLib {

    //! Chinese calendar
    /*! Holidays:
        <ul>
        <li>Saturdays</li>
        <li>Sundays</li>
        <li>New Year's day, January 1st (possibly followed by one or
            two more holidays)</li>
        <li>Labour Day, first week in May</li>
        <li>National Day, one week from October 1st</li>
        </ul>

        Other holidays for which no rule is given (data available for
        2004-2019 only):
        <ul>
        <li>Chinese New Year</li>
        <li>Ching Ming Festival</li>
        <li>Tuen Ng Festival</li>
        <li>Mid-Autumn Festival</li>
        <li>70th anniversary of the victory of anti-Japaneses war</li>
        </ul>

        SSE data from <http://www.sse.com.cn/>
        IB data from <http://www.chinamoney.com.cn/>

        \ingroup calendars
    */
    class ExChina : public Calendar {
      private:
        class SseImpl : public Calendar::Impl {
          public:
            std::string name() const { return "Shanghai stock exchange"; }
            bool isWeekend(Weekday) const;
            bool isBusinessDay(const Date&) const;
        };

        class SzseImpl : public Calendar::Impl {
          public:
            SzseImpl() {
                sseImpl = ext::shared_ptr<Calendar::Impl>(new ExChina::SseImpl);
            }
            std::string name() const { return "Shen Zhen stock exchange";}
            bool isWeekend(Weekday) const;
            bool isBusinessDay(const Date&) const;
          private:
            ext::shared_ptr<Calendar::Impl> sseImpl;
        };

        class IbImpl : public Calendar::Impl {
          public:
            IbImpl() {
                sseImpl = ext::shared_ptr<Calendar::Impl>(new ExChina::SseImpl);
            }
            std::string name() const { return "China inter bank market";}
            bool isWeekend(Weekday) const;
            bool isBusinessDay(const Date&) const;
          private:
            ext::shared_ptr<Calendar::Impl> sseImpl;
        };
      public:
        enum Market { SSE,    //!< Shanghai stock exchange
                      SZSE,   //!< Shen Zhen stock exchange
                      IB,     //!< Interbank calendar
        };
        ExChina(Market m = SSE);
    };

}


#endif
