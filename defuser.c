#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    count=1;
    int a16=(uint32_t)(challenge>>16);
    int High32=(uint32_t)(challenge>>32);
    int Low32=(uint32_t)(challenge&0xFFFFFFFF);
    values[0]=(uint32_t)(((Low32*4+High32)^0x4b4b)^0xf277) +(uint32_t)(((Low32-a16)*3+0xfbe)*8);
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t Left2,Right2;
    uint64_t Test2,Try2;
    Test2=((challenge>>0x28)&0xFF)*3;
    values[0]=(uint32_t)Test2;    
    count=6;               
    uint64_t Move2=0xc;                              
    Left2=(uint32_t)Test2;                                
    for(uint32_t i=1;i<=5;i++)                   
    {                                            
        Left2*=2;                                
        Try2=challenge>>(Move2&0xFF);            
        Right2=(uint32_t)(Try2&0x1F);            
        Left2=Left2^Right2;                      
        Right2=8*i+Left2;                                                   
        values[i]=Right2;
        Left2=Right2;   
        Move2+=3;                     
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {                                          
    uint32_t values[MAX_FIELDS] = {0};                                                                  
    size_t count = 0;                                                                                   
    (void)challenge;                                                                                    
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数。
    count=2;
    uint32_t Test31=(uint32_t)(challenge>>0x20);
    uint32_t Test32=(uint32_t)(challenge>>0x3);
    uint32_t Test3=Test31^Test32;
    uint32_t Last3=(uint32_t)challenge&0x7;
    uint64_t ReTest3=0;
    switch(Last3)
    {
        case 5:
            ReTest3=(Test3>>1)+(Test3^0xe8d6);
            break;
        case 1:
            ReTest3=(Test3*2)^0xdba6;
            break;
        case 2:
            ReTest3=Test3*5+0x31867;
            break;
        case 3:
            ReTest3=Test3*5+0x8d18;
            break;
        case 0:
            ReTest3=(Test3*2)^0x2729;
            break;
        case 4:
            ReTest3=Test3*5+0xed03;
             break;
        case 6:
            ReTest3=Test3*5+0x62a5;
            break;
        case 7:
            ReTest3=(Test3>>1)+(Test3^0x5b0b);
            break;
    }
    values[0]=Last3;
    values[1]=(uint32_t)ReTest3;
    fprintf(stderr, "challenge=0x%llx which=%u x=0x%x ret=0x%x\n",
        (unsigned long long)challenge, Last3, Test3, (uint32_t)ReTest3);
    return write_answer(out, cap, values, count);                                                       
}                                                                                                       

static int p4_walk(uint32_t r, uint32_t x, uint32_t c, uint32_t d) {
    uint32_t m=x+((c-x)>>1);   
    if (r==m) 
    {
        return (int)((m+0xa5u)^d);
    }

    uint32_t ebx=m+d;

    if (r>m) 
    {
        uint32_t rec=(uint32_t)p4_walk(r,m+1,c,d);
        return (int)(((rec*3u)^ebx)^0xau);
    } 
    else
    {
        uint32_t rec=(uint32_t)p4_walk(r,x,m-1,d);
        return (int)(((rec*3u)^ebx)^0x13u);
    }
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    uint32_t r=(uint32_t)(challenge&0x7fULL);
    uint32_t x=0;
    uint32_t c=0x7f;
    uint32_t d=(uint32_t)(((challenge>>32)^(challenge>>7))&0xffffffffULL);

    int target=p4_walk(r,x,c,d);

    values[0]=(uint32_t)target;
    count=1;

    return write_answer(out, cap, values, count);
}

struct p5_raw 
{
    int val0;      
    int val1;      
    int next_idx;  
};


static const struct p5_raw p5_raw[] = 
{
    { 0x8d8f, 0x0245,  8 },
    { 0xe1ef, 0x0520,  11 },
    { 0x8d9a, 0x036f,  0 },
    { 0x7097, 0x007c,   10 },
    { 0x64ca, 0x0b67,  7 },
    { 0x8fbd, 0x0a6e,  9 },
    { 0x790d, 0x0815,  3 },
    { 0x768c, 0x0919, -1 },
    { 0x5e1a, 0x0642,  1 },
    { 0x81e3, 0x0112, 2},
    { 0x5484, 0x073d, 4},
    {0xba31 ,0x0471, 6},
};


static const int p5_head_idx[8] = 
{
    5,   
    9,   
    2,   
    0,   
    8,   
    1,   
    11,  
    6,   
};

static uint32_t p5_traverse(uint64_t challenge, int *result_out) 
{
    uint32_t eax = (uint32_t)challenge;
    uint32_t sh3 = (uint32_t)(challenge >> 3);
    uint32_t hi  = (uint32_t)(challenge >> 32);
    uint32_t sh6 = (uint32_t)(challenge >> 6);
    unsigned idx = (uint32_t)challenge & 7u;
    int cur = p5_head_idx[idx];
    int esi = p5_raw[cur].val1;
    eax  = sh6 ^ hi;                     
    eax ^= (uint32_t)p5_raw[cur].val0;   
    eax ^= 0x49fau;                      
    *result_out = esi;                   
    eax += (uint32_t)esi;                
    uint32_t n = sh3 & 3u;               
    if (n == 0)
        return eax;                      
    uint32_t cnt = 1;                    
    do {
        cur = p5_raw[cur].next_idx;      
        cnt++;                           
        esi = p5_raw[cur].val1;          
        eax ^= (uint32_t)p5_raw[cur].val0;
        eax += (uint32_t)esi;            
        *result_out = esi;               
    } while (n >= cnt);                  
    return eax;                          
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) 
{
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    int result = 0;
    uint32_t ret = p5_traverse(challenge, &result);

    values[1] = (uint32_t)result;   
    values[0] = ret;                
    count = 2;

    return write_answer(out, cap, values, count);
}

static uint32_t p6_op_0(uint32_t x) { return (x + 0x4d1e) ^ (x >> 1); }
static uint32_t p6_op_1(uint32_t x) { return ((x << 5) | (x >> 27)) ^ 0xc795; }
static uint32_t p6_op_2(uint32_t x) { return (x * 9 + 0x6e8a) ^ (x * 3); }
static uint32_t p6_op_3(uint32_t x) { return (x ^ (x >> 7)) + 0x43cb; }
static uint32_t p6_op_4(uint32_t x) { return (x >> 2) | (x << 30); }
static uint32_t p6_op_5(uint32_t x) { return (x ^ 0x62ac) + (x << 5); 

static uint32_t (*p6_table[8])(uint32_t) = {
    p6_op_4,  /* idx=0 */
    p6_op_3,  /* idx=1 */
    p6_op_2,  /* idx=2 */
    p6_op_1,  /* idx=3 */
    p6_op_0,  /* idx=4 */
    p6_op_5,  /* idx=5 */
    p6_op_0,  /* idx=6 */
    p6_op_5,  /* idx=7 */
};

/* 对应汇编 0x402560 */
static uint32_t p6_dispatch(uint64_t arg) {
    uint32_t t1   = (uint32_t)(arg >> 6) ^ (uint32_t)(arg >> 32);
    uint32_t idx1 = (uint32_t)(arg & 0x7);          
    uint32_t idx2 = (uint32_t)((arg >> 3) & 0x7);   
    uint32_t r1   = p6_table[idx1](t1);
    r1 ^= 0xbe40;
    uint32_t r2   = p6_table[idx2](r1);
    return r2 + 0xa9e1;
}

/* ===== phase_06 主函数 ===== */
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;

    values[0] = p6_dispatch(challenge);
    count = 1;

    return write_answer(out, cap, values, count);
}

uint32_t secret_mix(uint32_t x) {
    return ((x >> 7) ^ 0xf3bb) + (x ^ 0xad7d);
}

int solve_secret(uint64_t challenge, uint64_t context,
                 char *out, size_t cap) {
    uint32_t ecx = (uint32_t)(context & 3);
    uint32_t r9d = ecx * 0xad7d;
    r9d ^= (uint32_t)(context >> 16);

    uint32_t esi = (uint32_t)((challenge >> 32) ^ (uint32_t)challenge);
    uint32_t r8d = (uint32_t)((context >> 32) ^ (uint32_t)context);
    uint32_t edi = esi ^ r8d;

    uint32_t eax   = secret_mix(edi) ^ r9d;
    uint32_t edx   = (uint32_t)(0xf3bb + r8d + ecx) + (esi >> 7);
    uint32_t third = eax ^ edx;

    int n = snprintf(out, cap, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN %u %u %u", ecx, eax, third);
    return (n >= 0 && (size_t)n < cap) ? 0 : 1;
}
