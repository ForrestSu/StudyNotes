//============================================================================
// @author  : sunquan
// @date   : 2019-09-25 18:44:22
//============================================================================

/**
业务： 质押式回购类 (交易所)
 1. 给定回购天数 清算速度， 计算到期结算日 首期结算日
 2. 给定回购天数 回购金额 利率 清算速度， 计算 回购利息
 3. 给定回购天数或回购代码 计算交易品种
 4. 给定回购天数 计算实际占款天数(交易日转自然日)
*/
#pragma once
#include <iostream>
#include <ql/quantlib.hpp>
#include "time/calendars/ex_china.hpp"
#include "dict_def.hpp"


using namespace QuantLib;
namespace PledgeRepo {


/**
 * 1 计算首期结算日
 */
inline Date CalcFirstSettlementDate(const Date &trade_date, Exchange::ClearSpeed tn = Exchange::ClearSpeed::T0)
{
    if (tn == Exchange::ClearSpeed::T0) {
        return trade_date;
    }
    // 使用上交所日历
    QuantLib::Calendar ib_calender = QuantLib::ExChina(ExChina::SSE);
    return ib_calender.adjust(trade_date + tn, BusinessDayConvention::Following);
}

/**
 * 1.1 计算到期结算日
 */
inline Date CalcExpireSettlementDate(const Date &trade_date, int repo_days, Exchange::ClearSpeed speed = Exchange::ClearSpeed::T0)
{
    Date expire_date = trade_date + repo_days;
    // 使用上交所日历
    QuantLib::Calendar ib_calender = QuantLib::ExChina(ExChina::SSE);
    return ib_calender.adjust(expire_date, BusinessDayConvention::Following);
}



/**
 * case 3. 给定回购天数(或回购代码) 计算交易品种
 */
inline Exchange::SSE::PLEDGE_BOND_REPO GetRepoByDays(int days)
{
   return Exchange::SSE::convert_days_to_enum(days);
}



/**
 * case 4. 给定回购天数 计算实际占款天数
 * 计算公式: 资金实际占款天数，是指当次回购交易的首次交收日（含）至到期交收日（不含）的实际日历天数。
 * @ref: 参考 https://www.jisilu.cn/question/93693
 */
int CapitalOccupationDays(const QuantLib::Date& trade_date, int repo_days)
{
    /**
     * 使用上交所交易日历
     * SSE SZSE
     */
    QuantLib::Calendar ib_calender = QuantLib::ExChina(ExChina::SSE);
    /**
     * (1) 计算融资方到账日 T+1
     */
    Date entry_date = ib_calender.adjust(trade_date + 1, BusinessDayConvention::Following);
    //std::cout << "entry_date == " << entry_date << std::endl;

    /**
     * (2) 计算到期日
     */
    Date expired_date = trade_date + repo_days;
    /**
     * (3) 计算到期清算日
     */
    Date expired_liquidate = ib_calender.adjust(expired_date, BusinessDayConvention::Following);

    /**
     * (4) 计算融券方资金到账日 = expired_liquidate + 1
     */
    Date entry_date_back = ib_calender.advance(expired_liquidate, 1, Days, BusinessDayConvention::Following);
    //std::cout << "entry_date_back == " << entry_date_back << std::endl;
    int diff_days = (entry_date_back - entry_date);
    return diff_days;
}


/**
 * case 2. 给定回购天数 回购金额 利率 清算速度 计算 回购利息
 *
 *   回购利息 = 交易金额*交易价格（即成交利率）/360 * 实际占款天数
 *   回购利息=
 *      (银行间)交易金额 * 回购利率 * 实际占款天数 / 365
 *      (交易所)交易金额* 回购利率 * 实际占款天数 / 360
 */

inline Decimal CalcRepurchaseInterest(const QuantLib::Date& trade_date,
        Decimal turnover,
        Decimal rate,
        int repo_days,
        Exchange::ClearSpeed speed = Exchange::ClearSpeed::T0)
{
    int occupation_days = CapitalOccupationDays(trade_date, repo_days);
    Decimal interest = (turnover * rate * occupation_days) / 360;
    return interest;
}



void run()
{

    // test case 4
    {
        Date trade_date(25, Sep, 2019);
        std::vector<int> short_repo { 1, 2, 3, 4, 7, 14, 28, 91, 182 };
        std::vector<int> ans        { 1, 4,12,12,13, 14, 28, 91, 182 };
        for (size_t k = 0; k < short_repo.size(); ++k) {
            int diff_days = CapitalOccupationDays(trade_date, short_repo[k]);
            std::cout << short_repo[k] << ", 实际占款天数: " << diff_days << (ans[k] == diff_days ? " true" : " false") << std::endl;
        }
    }
    {
        Date trade_date(26, Sep, 2019);
        std::vector<int> short_repo { 1, 2, 3, 4, 7, 14, 28, 91, 182 };
        std::vector<int> ans        { 3, 11,11,11,12,14, 28, 91, 182 };
        for (size_t k = 0; k < short_repo.size(); ++k) {
            int diff_days = CapitalOccupationDays(trade_date, short_repo[k]);
            std::cout << short_repo[k] << ", 实际占款天数: " << diff_days << (ans[k] == diff_days ? " true" : " false") << std::endl;
        }
    }

    // test case 2
    {
        Date trade_date(27, Sep, 2019);
        std::vector<int> short_repo { 1, 2, 3, 4, 7, 14, 28, 91, 182 };
        std::vector<int> ans        { 8, 8, 8, 9, 9, 14, 28, 91, 182 };
        // 百元收益率
        std::vector<double> rates    { 1.86, 1.99, 2.05, 2.58, 2.66, 2.765, 2.780, 2.885, 2.870};
        // 百元手续费
        std::vector<double> fees     { 0.001, 0.002, 0.003, 0.004, 0.005, 0.010, 0.020, 0.030, 0.030 };
        for (size_t k = 0; k < short_repo.size(); ++k) {
            int diff_days = CapitalOccupationDays(trade_date, short_repo[k]);
            double turnover = 100000;
            Decimal interest = CalcRepurchaseInterest(trade_date, turnover/100, rates[k], short_repo[k]);
            double tmp_fee = fees[k] * turnover/100;
            double payback = interest - tmp_fee;
            std::cout << short_repo[k] << " 天期, 占款天数" << diff_days << ", 金额10W, 回购利息: " << interest << ", 预期收益: "<< payback << std::endl;
        }
    }

}

}
