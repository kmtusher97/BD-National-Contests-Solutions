"""BISMILLAHIR RAHMANIR RAHIM"""

import math

mxn = 100005
vis = [False] * mxn
primes = []
factors = [[]]


def sieve():
    vis[0] = vis[1] = True
    for i in range(2, mxn):
        if not vis[i]:
            primes.append(i)
            for j in range(i + i, mxn, i):
                vis[j] = True


def prime_factorize():
    for i in range(1, mxn):
        n = i
        factors.append([])
        for p in primes:
            if p > n:
                break
            tmp = 1
            while n > 0 and n % p == 0:
                n /= p
                tmp *= p

            if tmp == 1:
                continue
            factors[i].append(tmp)

        if n > 1:
            factors[i].append(n)


def extended_euclid(a, b):
    if a == 0:
        return b, 0, 1
    else:
        gcd, x, y = extended_euclid(b % a, a)
        return gcd, y - (b // a) * x, x


def chinese_reminder_theorem(A, P):
    if len(A) == 0 or len(A) != len(P):
        return 0, 0

    a1 = A[0]
    m1 = P[0]
    for i in range(1, len(A)):
        a2 = A[i]
        m2 = P[i]
        g = math.gcd(m1, m2)
        if a1 % g != a2 % g:
            return -1, -1
        gcd, p, q = extended_euclid(m1 // g, m2 // g)
        mod = (m1 // g) * m2
        x = (a1 * (m2 // g) * q + a2 * (m1 // g) * p) % mod
        a1 = x
        if a1 < 0:
            a1 += mod
        m1 = mod

    return a1, m1


def main():
    sieve()
    prime_factorize()

    cases = int(input())
    for case_no in range(1, cases + 1):
        N, M = map(int, input().split())
        res = [-1] * (M + 1)
        res[0] = res[1] = 0
        for p in primes:
            if p > M:
                break
            max_p = 1
            while max_p * p <= M:
                max_p *= p

            res[max_p] = N % max_p
            i = max_p // p
            while i >= p:
                res[i] = res[max_p] % i
                i //= p

        for i in range(M, 1, -1):
            if res[i] != -1:
                continue
            if i * 2 <= M:
                res[i] = res[i * 2] % i
            else:
                P = factors[i]
                A = []
                for fct in factors[i]:
                    A.append(res[fct])
                tmp_x, tmp_m = chinese_reminder_theorem(A, P)
                res[i] = tmp_x

        ans = sum(res)
        print('Case %d: %d' % (case_no, ans))


main()