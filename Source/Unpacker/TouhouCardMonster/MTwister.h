#ifndef _MTWISTER_H_5cf039de_248a_4c8d_b583_90698d300c3e_
#define _MTWISTER_H_5cf039de_248a_4c8d_b583_90698d300c3e_

#include "MyLibrary.h"

// The Mersenne Twister

class CMTwister
{
protected:
    UInt32 m_Twist[624];
    UInt32 m_Count;

public:
    CMTwister()
    {
        m_Count = 625;
    }

    CMTwister(UInt32 Seed)
    {
        SetSeed(Seed);
    }

    Void SetSeed(UInt32 Seed)
    {
        UInt32  Temp;
        PUInt32 pTwist;

        pTwist = m_Twist;
        for (UInt32 i = countof(m_Twist); i; --i)
        {
            Temp = Seed;
            Seed = Seed * 0x10DCD + 1;
            *pTwist++ = (Temp & 0xFFFF0000) | (Seed >> 16);
            Seed = Seed * 0x10DCD + 1;
        }

        m_Count = countof(m_Twist);
    }

    VOID SetSeed2(ULONG Seed)
    {
        ULONG   Temp;
        PULONG  Twist;

        Twist = (PULONG)m_Twist;
        *Twist++ = Seed;
        for (ULONG_PTR Index = 1, Count = countof(m_Twist) - 1; Count; ++Index, --Count)
        {
            Temp = Twist[-1];
            *Twist++ = (Temp ^ (Temp >> 30)) * 0x6C078965 + Index;
        }

        m_Count = countof(m_Twist);
    }

    LARGE_INTEGER GetRandomInternal()
    {
        UInt32  Index, val;
        PUInt32 Twist;

#define GET_MASK(val) (((val) & 1) ? 0x9908B0DF : 0)

        if (m_Count >= countof(m_Twist))
        {
            Twist = m_Twist;

            for (Index = 0; Index != 0xE3; ++Twist, ++Index)
            {
                val = Twist[1];
                val = (val ^ Twist[0]) & 0x7FFFFFFF;
                val ^= Twist[0];
                val = (val >> 1) ^ GET_MASK(val);
                val ^= Twist[396 + 1];
                Twist[0] = val;
            }

            while (Index < countof(m_Twist) - 1)
            {
                val = (Twist[0] ^ Twist[1]) & 0x7FFFFFFF;
                val ^= Twist[0];
                val = (val >> 1) ^ (GET_MASK(val) ^ Twist[-0xE4 + 1]);
                Twist[0] = val;

                ++Twist;
                ++Index;
            }

            val = m_Twist[0] ^ m_Twist[countof(m_Twist) - 1];
            val &= 0x7FFFFFFF;
            val ^= m_Twist[countof(m_Twist) - 1];
            val = (val >> 1) ^ GET_MASK(val);
            m_Twist[countof(m_Twist) - 1] = val ^ m_Twist[396];
            Index = 0;
        }
        else
        {
            Index = m_Count;
        }

#undef GET_MASK

        val = m_Twist[Index++];
        m_Count = Index;
        val ^= val >> 0x0B;
        val ^= (val & 0xFF3A58AD) << 0x07;
        val ^= (val & 0xFFFFDF8C) << 0x0F;

        LARGE_INTEGER ret;

        ret.HighPart = val;
        ret.LowPart = val >> 0x12;

        return ret;
    }

    ULONG GetRandom()
    {
        return GetRandomInternal().LowPart;
    }

    LARGE_INTEGER GetRandom64()
    {
        return GetRandomInternal();
    }
};

#endif // _MTWISTER_H_5cf039de_248a_4c8d_b583_90698d300c3e_
