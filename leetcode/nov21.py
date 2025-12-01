def gcd(a,b):
    if b == 0:
        return a
    # a has to be bigger
    return gcd(b, a % b)

def reduce(num, denom): # return reduced form string from num and denom
    gcd_ = gcd(max(num, denom), min(denom, num))
    return f"{int(num/gcd_)}/{int(denom/gcd_)}"

def solution(s):
    terms = s.split('+')

    x = int(terms[0].split('/')[0])
    y = int(terms[0].split('/')[1])

    u = int(terms[1].split('/')[0])
    v = int(terms[1].split('/')[1])

    num1 = x * v
    num2 = y * u
    denom = y * v

    frac = f"{num1 + num2}/{denom}"
    print(f"unreduced fraction: {frac}")
    return reduce(num1+num2, denom)


def main():
    



main()