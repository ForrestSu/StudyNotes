//============================================================================
// @author  : 彭士杰
// @date   : 2019-09-25 18:44:22
//============================================================================

/**
 业务： 买断式回购
 1. 给定回购利率 回购天数 首期净价 券面总额， 计算首次收益率、到期净价、到期收益率、首期应计利息、到期应计利息
 2. 给定回购天数、清算速度, 计算到期结算日 首期结算日
 3. 给定回购天数, 计算交易品种
 4. 给定回购天数, 计算实际占款天数
 */
#pragma once
#include <iostream>
#include <ql/quantlib.hpp>
#include <string> 
#include "time/calendars/ex_china.hpp"
#include "dict_def.hpp"

using namespace QuantLib;
namespace OutRightRepo {

/**
 * 新规何时开始实施?新规实施以前达成但尚未到期回购如何计息?
 *   新规将于2017年5月22日起实施。
 *   对投资者在2017年5月22日前已达成回购交易的到期购回价仍按目前的计息规则(即回购名义天数和全年360天)计算;
 *   在新规实施当日(含)后发生的回购交易则按照实际资金占款天数和全年365天来计息。
 *
 * 名词解释：
 *  回购利率：根据首次结算金额、到期结算金额和回购期限等要素计算的参考利率，回购利率不可为负。
 *  首次净价：正回购方向逆回购方卖出债券的净价，单位为元/百元面值。
 *  到期净价：正回购方向逆回购方买回债券的净价，单位为元/百元面值。
 */
    /**
     * case 1. 给定回购利率 回购天数 首期净价 券面总额， 计算首次收益率、到期净价、到期收益率、首期应计利息、到期应计利息
     */
    /**
     *
     * @param first_net_price 首次净价
     * @param repo_rate 回购利率
     *
     * 到期净价 = 首期净价+每百元的回购利息
     * 到期收益率 = 不同的债券类型收益率计算公式不一样,直接用quantlib可以使用
     */
    inline void get_interest(double first_net_price, Rate repo_rate)
    {

    }

    /**
     *  case 2 计算首次结算日 （默认清算速度T+1）
     * 计算公式： 首次结算日=成交日+清算速度
     */
    inline Date CalcFirstSettlementDate(const QuantLib::Date trade_date, Exchange::ClearSpeed speed = Exchange::ClearSpeed::T1)
    {
        QuantLib::Calendar ib_calender = QuantLib::ExChina(ExChina::IB);
        return ib_calender.adjust(trade_date + speed, QuantLib::BusinessDayConvention::Following);
    }

    /**
     * case 2.1 计算到期结算日
     * 计算公式： 到期结算日=首次结算日+回购期限（遇到节假日顺延至下一工作日）
     */
    inline Date CalcExpireSettlementDate(const QuantLib::Date first_settlement_date, int repo_days)
    {
        QuantLib::Calendar ib_calender = QuantLib::ExChina(ExChina::IB);     
        return ib_calender.adjust(first_settlement_date + repo_days, QuantLib::BusinessDayConvention::Following);
    }

    /**
     * case 3. 给定回购天数或回购代码, 计算交易品种
     */
    inline Exchange::IB::OUTRIGHT_REPO get_InnerBank_kind(int days)
    {
        return Exchange::IB::convert_days_to_enum(days);
    }


    /**
     * case 4. 计算实际占款天数
     * 计算公式： 实际占款天数=到期结算日-首次结算日
     */
    inline Integer CapitalOccupationDays(const QuantLib::Date trade_date, int repo_days, Exchange::ClearSpeed speed = Exchange::ClearSpeed::T1)
    {
        Date first_settlement_date = CalcFirstSettlementDate(trade_date, speed);
        Date expired_date = CalcExpireSettlementDate(first_settlement_date, repo_days);
        int diff_days = expired_date - first_settlement_date;
        return diff_days;
    }

    inline void run()
    {
        //test case 2
        QuantLib::Date trade_date(14, March, 2019);
        // 回购天数
        const int repo_days = 30;
        std::cout << "买断式回购天数：" << repo_days << "，交易日期：" << trade_date << std::endl;
        Date first_settlement_date = CalcFirstSettlementDate(trade_date);
        std::cout << "> 首次结算日 : " << first_settlement_date << std::endl;
        Date expired = CalcExpireSettlementDate(first_settlement_date, repo_days);
        std::cout << "> 到期结算日 : " << expired << std::endl;

        //test case 3
        Exchange::IB::OUTRIGHT_REPO type = get_InnerBank_kind(repo_days);
        std::string repo_kind = Exchange::IB::outright_repo_to_string(type);
        std::cout << "> repo_days : " << repo_days << ", 品种代码为: " << repo_kind << std::endl;

        //test case 4
        int diff_days = CapitalOccupationDays(trade_date, repo_days);
        std::cout << "> 实际占款天数 : " << diff_days << std::endl;
        std::cout << std::endl;
    }

}
