import sys

def main():
    s = sys.argv[1]
    print(testPalindrome(s))

def testPalindrome(s: str) -> bool:
    i = 0
    j = len(s) - 1

    while i <= j:
        print(f"s[{i}]={s[i]}, s[{j}]={s[j]}")
        if s[i].isalpha() and s[j].isalpha():
    
            if s[i].lower() != s[j].lower():
                return False
            i+= 1
            j -= 1
            continue

        elif s[i].isalpha() and not s[j].isalpha():
            j -= 1
            continue
            
        elif not s[i].isalpha() and s[j].isalpha():
            i += 1
            continue

        else:
            i += 1
            j -= 1
            continue
    return True

if __name__ == "__main__":
    main()
