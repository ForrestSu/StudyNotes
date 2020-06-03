//============================================================================
// @create : sunquan
// @date   : 2019-09-27 14:34:39
//============================================================================
#pragma once
#include <string>


namespace Exchange {

// Market 采用 ExChina 内部定义的 market

/**
 * 上交所/深交所-交易品种定义
 * (1) 短期质押式国债逆回购 ： 上交所 深交所 各9只
 */

/**
 * 清算速度
 */
enum ClearSpeed {
    T0 = 0, //!< T0 交易所一般为 T+0
    T1 = 1, //!< T1 银行间一般为T+1
};


///上交所
namespace SSE {
    /**
     上交所回购品种：
     1天国债回购（GC001，代码204001）
     2天国债回购（GC002，代码204002）
     3天国债回购（GC003，代码204003）
     4天国债回购（GC004，代码204004）
     7天国债回购（GC007，代码204007）
     14天国债回购（GC014，代码204014）
     28天国债回购（GC028，代码204028）
     91天国债回购（GC091，代码204091）
     182天国债回购（GC182，代码204182）
     */
    enum PLEDGE_BOND_REPO {
        UNKNOWN_REPO = 0, //!< UNKNOWN
        GC001 = 1,      //!< 204001
        GC002 = 2,      //!< 204002
        GC003 = 3,      //!< 204003
        GC004 = 4,      //!< 204004
        GC007 = 5,      //!< 204007
        GC014 = 6,      //!< 204014
        GC028 = 7,      //!< 204028
        GC091 = 8,      //!< 204091
        GC182 = 9,      //!< 204182
    };
    inline const std::string pledge_bond_repo_to_string(PLEDGE_BOND_REPO type)
    {
        static const std::string dict[] = { "UNKNOWN", "GC001", "GC002", "GC003", "GC004", "GC007", "GC014", "GC028", "GC091", "GC182" };
        static const size_t len = sizeof(dict) / sizeof(dict[0]);
        return (type < len ? dict[type] : dict[0]);
    }

    inline PLEDGE_BOND_REPO convert_days_to_enum(int days)
    {
        switch (days) {
        case 1: return PLEDGE_BOND_REPO::GC001;
        case 2: return PLEDGE_BOND_REPO::GC002;
        case 3: return PLEDGE_BOND_REPO::GC003;
        case 4: return PLEDGE_BOND_REPO::GC004;
        case 7: return PLEDGE_BOND_REPO::GC007;
        case 14: return PLEDGE_BOND_REPO::GC014;
        case 28: return PLEDGE_BOND_REPO::GC028;
        case 91: return PLEDGE_BOND_REPO::GC091;
        case 182: return PLEDGE_BOND_REPO::GC182;
        default: return PLEDGE_BOND_REPO::UNKNOWN_REPO;
        }
    }
}


///深交所
namespace SZSE {

    /**
     1 天国债回购（R－001 代码131810）
     2 天国债回购（R－002 代码131811）
     3天国债回购（R－003 代码131800）
     4天国债回购（R－004 代码131809）
     7天国债回购（R－007代码131801）
     14天国债回购（R－014 代码131802）
     28天国债回购（R－028 代码131803）
     91天国债回购（R－091 代码131805）
     182天国债回购（R－182 代码131806）
     */
    enum PLEDGE_BOND_REPO {
        UNKNOWN_REPO = 0, //!< UNKNOWN
        R_001 = 1,     //!< 131810
        R_002 = 2,     //!< 131811
        R_003 = 3,     //!< 131800
        R_004 = 4,     //!< 131809
        R_007 = 5,     //!< 131801
        R_014 = 6,     //!< 131802
        R_028 = 7,     //!< 131803
        R_091 = 8,     //!< 131805
        R_182 = 9,     //!< 131806
    };
    inline const std::string pledge_bond_repo_to_string(PLEDGE_BOND_REPO type)
    {
        static const std::string dict[] = { "UNKNOWN", "R-001", "R-002", "R-003", "R-004", "R-007", "R-014", "R-028", "R-091", "R-182"};
        static const size_t len = sizeof(dict) / sizeof(dict[0]);
        return (type < len ? dict[type] : dict[0]);
    }
    inline PLEDGE_BOND_REPO convert_days_to_enum(int days)
    {
        switch (days) {
        case 1: return PLEDGE_BOND_REPO::R_001;
        case 2: return PLEDGE_BOND_REPO::R_002;
        case 3: return PLEDGE_BOND_REPO::R_003;
        case 4: return PLEDGE_BOND_REPO::R_004;
        case 7: return PLEDGE_BOND_REPO::R_007;
        case 14: return PLEDGE_BOND_REPO::R_014;
        case 28: return PLEDGE_BOND_REPO::R_028;
        case 91: return PLEDGE_BOND_REPO::R_091;
        case 182: return PLEDGE_BOND_REPO::R_182;
        default: return PLEDGE_BOND_REPO::UNKNOWN_REPO;
        }
    }
}

//银行间
namespace IB {
    /**
    * 银行间买断品种
    */
    enum OUTRIGHT_REPO {
        UNKNOWN_KIND = 0,
        R001 = 1,     //1天期
        R007 = 2,     //2至7天期
        R014 = 3,    //8至14天期
        R021 = 4,    //15至21天期
        R1M = 5,     //22至30天期
        R2M = 6,     //31至60天期
        R3M = 7,     //61至90天期
        R4M = 8,    //91至120天期
        R6M = 9,    //121至180天期
        R9M = 10,    //181至270天期
        R1Y = 11,    //271至365天期
    };
    inline std::string outright_repo_to_string(OUTRIGHT_REPO type)
    {
        static const std::string dict[] = { "UNKNOWN", "R001", "R007", "R014", "R004", "R021", "R1M", "R2M", "R3M", "R4M", "R6M", "R9M", "R1Y" };
        static const size_t len = sizeof(dict) / sizeof(dict[0]);
        return (type < len ? dict[type] : dict[0]);
    }
    inline OUTRIGHT_REPO convert_days_to_enum(int days)
    {
        if (days == 1) {
            return OUTRIGHT_REPO::R001;
        }
        else if (days > 1 && days <= 7) {
            return OUTRIGHT_REPO::R007;
        }
        else if (days > 7 && days <= 14) {
            return OUTRIGHT_REPO::R014;
        }
        else if (days > 14 && days <= 21) {
            return OUTRIGHT_REPO::R021;
        }
        else if (days > 21 && days <= 30) {
            return OUTRIGHT_REPO::R1M;
        }
        else if (days > 30 && days <= 60) {
            return OUTRIGHT_REPO::R2M;
        }
        else if (days > 60 && days <= 90) {
            return OUTRIGHT_REPO::R3M;
        }
        else if (days > 90 && days <= 120) {
            return OUTRIGHT_REPO::R4M;
        }
        else if (days > 120 && days <= 180) {
            return OUTRIGHT_REPO::R6M;
        }
        else if (days > 180 && days <= 270) {
            return OUTRIGHT_REPO::R9M;
        }
        else if (days > 270 && days <= 365) {
            return OUTRIGHT_REPO::R1Y;
        }
        else {
            return OUTRIGHT_REPO::UNKNOWN_KIND;
        }
    }
}

}
