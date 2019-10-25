//============================================================================
// @author  : sunquan
// @date   : 2019-09-25 18:44:22
//============================================================================

/**
 业务：现券类
 1. 给定债券代码和债券市场 计算债券下一付息日
 2. 给定债券代码和债券市场 付息周期 债券类型 计算债券久期 凸性
 3. 给定债券代码和债券市场 清算速度 净价 计算 到期收益率 行权收益率
 4. 给定债券代码和债券市场 清算速度 券面总额 净价 计算 结算日期
 5. 给定债券代码和债券市场 清算速度 券面总额 净价 计算 应计利息
 6. 给定债券代码和债券市场，计算持有到期的利息
 */

#pragma once
#include <iostream>
#include <ql/quantlib.hpp>
#include "time/calendars/ex_china.hpp"
#include "dict_def.hpp"

/**
 eg: 19国债06 019616
 挂牌代码:   019616
 挂牌名称:   19国债06
 国债全称:   2019年记账式附息(六期)国债
 实际发行量:  1840.9
 发行价格:   100
 发行方式:   记帐式
 期限: 10
 到期日:    2029-05-23
 票面利率:   3.29
 国债付息方式: 按半年付息
 计息日期:   2019-05-23
 上市日期:   2019-05-27
 */

namespace CashBond {


using namespace QuantLib;

/**
 * case 1.  给定债券代码和债券市场 计算债券下一付息日
 */
Date BondNextCashFlowDate(ExChina::Market mkt, Date issue_date, Date maturity_date, Frequency freq, Exchange::ClearSpeed speed = Exchange::ClearSpeed::T0)
{
    // 上交所日历
    Calendar sse_calender = ExChina(mkt);
    // 清算速度
    Natural settlement_days = speed;
    // 面额
    Real face_amount = 100.0;
    // 365Fix
    DayCounter dc =  Actual365Fixed();

    Real bond_redemption = 100.0;

    Schedule schedule = Schedule(issue_date, maturity_date,  Period(freq), sse_calender, Unadjusted, Unadjusted, DateGeneration::Forward, false);

    Bond bond = FixedRateBond(settlement_days,
            face_amount,
            schedule,
            std::vector<Rate>(1, 0.0329),
            dc,
            Following,
            bond_redemption,
            issue_date);
    // std::cout << "下一付息日 == " << bond.nextCashFlowDate() << std::endl;
    return bond.nextCashFlowDate();
}

/**
 * 计算上一个付息日
 */
Date BondPrevCashFlowDate(ExChina::Market mkt, Date issue_date, Date maturity_date, Frequency freq, Exchange::ClearSpeed speed = Exchange::ClearSpeed::T0)
{
    // 上交所日历
    Calendar sse_calender = ExChina(mkt);
    // 清算速度
    Natural settlement_days = speed;
    // 面额
    Real face_amount = 100.0;
    // 365Fix
    DayCounter dc =  Actual365Fixed();

    Real bond_redemption = 100.0;

    Schedule schedule = Schedule(issue_date, maturity_date,  Period(freq), sse_calender, Unadjusted, Unadjusted, DateGeneration::Forward, false);

    Bond bond = FixedRateBond(settlement_days,
            face_amount,
            schedule,
            std::vector<Rate>(1, 0.0329),
            dc,
            Following,
            bond_redemption,
            issue_date);
    // std::cout << "上一付息日 == " << bond.nextCashFlowDate() << std::endl;
    return bond.previousCashFlowDate();
}


/**
 * case 2. 给定债券代码和债券市场 付息周期 债券类型 ==> 计算债券久期 凸性
 */
void case2_duration_convexity()
{
    // 上交所日历
    Calendar sse_calender = ExChina(ExChina::SSE);
    // 发行日
    Date issue_date(23, May, 2019);
    // 到期日
    Date maturity_date(23, May, 2029);

    // 清算速度 T+0
    Natural settlement_days = 0;
    // 面额
    Real face_amount = 100;

    // 计息方式： 复利
    Compounding comp = Compounded;

    //付息方式：一年2次
    Frequency freq = Semiannual;

    Schedule schedule = Schedule(issue_date, maturity_date, Period(freq), sse_calender, Unadjusted, Unadjusted, DateGeneration::Forward,
            false);
    /**
     * 365Fix
     */
    DayCounter day_counter = Actual365Fixed();

    FixedRateBond bond = FixedRateBond(settlement_days, face_amount, schedule, std::vector<Rate>(1, 0.0329), day_counter,
            Following, 100.0, issue_date);

    const Leg& leg = bond.cashflows();

    // 获取结算日
    Date settlement_date = bond.settlementDate();

    // 应计数量
    Real accrued = bond.accruedAmount();

    Real test_price = 103.0;

    Real npv = test_price + accrued;

    // 1 先计算到期收益率
    Rate yield = CashFlows::yield(leg, npv, day_counter, comp, freq, false, settlement_date);
    // 2 计算久期
    Time duration = CashFlows::duration(leg, yield, day_counter, comp, freq, Duration::Modified, false, settlement_date);

    // 3 凸性
    Real convexity = CashFlows::convexity(leg, yield, day_counter, comp, freq, false, settlement_date);

    // 4 净现值
    Real calcnpv = CashFlows::npv(leg, yield, day_counter, comp, freq, false, settlement_date);

    std::cout << "应计数量  accrued == " << accrued << std::endl;
    std::cout << "结算日 ==  " << settlement_date << std::endl;
    std::cout << "收益率 yield ==  " << yield << std::endl;
    std::cout << "久期 duration ==  " << duration << std::endl;
    std::cout << "凸性 convexity ==  " << convexity << std::endl;
    std::cout << "净现值 == " << calcnpv << std::endl << std::endl;
}

/**
 * case 3.  给定债券代码和债券市场 清算速度 净价 计算 到期收益率 行权收益率
 * 注意： 行权收益率——取债券信息字段
 */
/**
 *
 * @param comp 计息方式：复利 Compounded
 */
double CalcExpiredRate(Compounding comp)
{
    // 上交所日历
    Calendar sse_calender = China(China::SSE);
    // 发行日
    Date issue_date(23, May, 2019);
    // 到期日
    Date maturity_date(23, May, 2029);

    // 清算速度
    Natural settlement_days = 0;
    // 面额
    Real face_amount = 100;

    //付息方式：一年2次
    Frequency freq = Semiannual;
    /**
     * 365Fix
     */
    DayCounter dc = Actual365Fixed();

    Schedule schedule = Schedule(issue_date, maturity_date, Period(freq), sse_calender, Unadjusted, Unadjusted, DateGeneration::Forward, false);

    FixedRateBond bond = FixedRateBond(settlement_days,
            face_amount,
            schedule,
            std::vector<Rate>(1, 0.0329),
            dc,
            Following,
            100.0,
            issue_date);

     // 计算净现值
    Real accrued = bond.accruedAmount();
    Real test_price = 103.0;
    Real npv = test_price + accrued;


    const Leg& leg = bond.cashflows();
    Rate yield_to_maturity = CashFlows::yield(leg, npv, dc, comp, freq, false, bond.settlementDate());
    std::cout << "到期收益率 == " << yield_to_maturity << std::endl;
    return yield_to_maturity;

    // 计算净现值
    // Real accrued = bond.accruedAmount();
    // Real test_price = 103.0;
    // Real npv = test_price + accrued;
}

/**
 * case 4. 计算 结算日期
 *  结算日期 = adjust(交易日期+清算速度)
 *   calendar = China(China::SSE);
 */
inline Date CalcSettlementDate(Calendar calendar, Date trade_date, Exchange::ClearSpeed speed)
{
    return calendar.adjust(trade_date + speed, BusinessDayConvention::Following);
}


/**
 * case 5. 给定债券代码和债券市场 清算速度 券面总额 净价 计算 应计利息
 *
 * 应计利息是指自上一利息支付日至买卖结算日产生的利息收入，
 * 具体而言:
 * (1) 零息债券是指发行起息日至交割日所含利息金额；
 * (2) 附息债券是指本付息期起息日至交割日所含利息金额；
 * (3) 贴现债券没有票面利率，其应计利息额设为零。
 * 应计利息的计算公式如下（以每百元债券所含利息额列示）： 应计利息额=票面利率÷365×已计息天数×券面总额
 */
double BondAccruedInterest(ExChina::Market mkt, Date trade_date, Real money, Date issue_date, Date maturity_date, Frequency freq, Real rate, Exchange::ClearSpeed speed)
{
    // 应计利息 = 以小数表示的息票利率 * 计息期年分数 * 面值
    // 1 先计算 计息天数
    Calendar calendar = ExChina(mkt);
    // 买卖结算日
    Date settle_date = CalcSettlementDate(calendar, trade_date, speed);
    // 上一个付息日
    Date prev_date = BondPrevCashFlowDate(mkt, issue_date, maturity_date, freq, speed);
    int interest_days = settle_date - prev_date;

    std::cout << "interest_days == " << interest_days << std::endl;
    //应计利息
    double interest = money * rate * interest_days / 365.0;
    return interest;
}

/** 仅支持 固定利率债券
 * case 6. 给定债券代码和债券市场，计算持有到期的利息
 */
double CalcExpiredInterest(ExChina::Market mkt, Date trade_date, Real money, Date maturity_date, Real rate, Exchange::ClearSpeed speed)
{
     // 应计利息 = 以小数表示的息票利率 * 计息期年分数 * 面值
    // 1 先计算 计息天数
    Calendar calendar = ExChina(mkt);
    // 买卖结算日
    Date settle_date = CalcSettlementDate(calendar, trade_date, speed);
    int days = maturity_date - settle_date;

     std::cout << "days == " << days << std::endl;
    // 持有到期的利息
    double interest = money * rate * days / 365.0;
    return interest;
}

void run()
{
    // case 1 计算下一付息日
    {
        Date issue_date(23, May, 2019);// 发行日
        Date maturity_date(23, May, 2029); // 到期日
        Frequency freq = Monthly; //频率
        Date next_date = BondNextCashFlowDate(ExChina::SSE, issue_date, maturity_date, freq);
        std::cout << "下一付息日 == " << next_date << std::endl;
    }

    // case 1.1 计算上一付息日
    {
        Date issue_date(23, May, 2019);// 发行日
        Date maturity_date(23, May, 2029); // 到期日
        Frequency freq = Monthly; //频率
        Date prev_date = BondPrevCashFlowDate(ExChina::SSE, issue_date, maturity_date, freq);
        std::cout << "上一付息日 == " << prev_date << std::endl;
    }

    // case 5 计算应计利息
    {
        Date issue_date(23, May, 2019);// 发行日
        Date maturity_date(23, May, 2029); // 到期日
        Frequency freq = Monthly; //频率
        Date trade_date = Date(14, October, 2019);
        Real money = 100000; //10W
        Real rate = 0.10; // 利率 10%
        double interest = BondAccruedInterest(ExChina::SSE, trade_date, money, issue_date, maturity_date, freq, rate, Exchange::ClearSpeed::T0);
        std::cout << "应计利息 == " << interest << std::endl;
    }

    {
        Date maturity_date(23, May, 2029); // 到期日
        Date trade_date = Date(14, October, 2019);
        Real money = 100000; //10W
        Real rate = 0.10; // 利率 10%
        double interest = CalcExpiredInterest(ExChina::SSE, trade_date, money, maturity_date, rate, Exchange::ClearSpeed::T0);
        std::cout << "持有到期利息 == " << interest << std::endl;
    }

    case2_duration_convexity();
    CalcExpiredRate(Compounded);
}

}
