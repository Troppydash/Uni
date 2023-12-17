import random
import os
import filecmp
import time

mode = 'release'

def write(lines, filename):
      with open(filename, 'w') as f:
        f.write('\n'.join(lines))


def addition():
    vars = "abcdefghijklmnopqrstuvwxyz"
    inputs = []
    expected = []
    for i in range(1000):
        num = random.randint(0, 9*10**498)
        var = random.choice(vars)
        inputs.append(f'{var}={num}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}={num}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num:,}')

        num1 = random.randint(0, 4*10**498)
        num2 = random.randint(0, 4*10**498)
        inputs.append(f'{var}={num1}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}={num1}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num1:,}')

        inputs.append(f'{var}+{num2}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}+{num2}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num1+num2:,}')

        inputs.append(f'{var}+{var}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}+{var}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {2*(num1+num2):,}')


    for i in range(500):
        num = random.randint(0, 10)
        var = random.choice(vars)
        inputs.append(f'{var}={num}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}={num}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num:,}')

        num1 = random.randint(0, 500)
        num2 = random.randint(0, 500)
        inputs.append(f'{var}={num1}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}={num1}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num1:,}')

        inputs.append(f'{var}+{num2}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}+{num2}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {num1+num2:,}')

        inputs.append(f'{var}+{var}')
        inputs.append(f'{var}?')
        expected.append(f'> {var}+{var}')
        expected.append(f'> {var}?')
        expected.append(f'register {var}: {2*(num1+num2):,}')

    expected.append('> ')
    expected.append('ta daa!!!')
    expected.append('')

    write(inputs, f'cmake-build-{mode}/input.txt')
    write(expected, f'cmake-build-{mode}/expected.txt')

def multiplication():
    vars = "abcdefghijklmnopqrstuvwxyz"
    inputs = []
    expected = []
    for i in range(500):
        var1, var2 = random.sample(vars, 2)
        num1 = random.randint(0, 4*10**210)
        num2 = random.randint(0, 4*10**280)

        inputs.append(f'{var1}={num1}')
        inputs.append(f'{var2}={num2}')
        expected.append(f'> {var1}={num1}')
        expected.append(f'> {var2}={num2}')

        inputs.append(f'{var1}*{var2}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}*{var2}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num1*num2:,}')

        num3 = random.randint(0, 4*10**210)
        inputs.append(f'{var2}*{num3}')
        inputs.append(f'{var2}?')
        expected.append(f'> {var2}*{num3}')
        expected.append(f'> {var2}?')
        expected.append(f'register {var2}: {num2*num3:,}')

    for i in range(500):
        var1, var2 = random.sample(vars, 2)
        num1 = random.randint(0, 500)
        num2 = random.randint(0, 500)

        inputs.append(f'{var1}={num1}')
        inputs.append(f'{var2}={num2}')
        expected.append(f'> {var1}={num1}')
        expected.append(f'> {var2}={num2}')

        inputs.append(f'{var1}*{var2}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}*{var2}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num1*num2:,}')

        num3 = random.randint(0, 10)
        inputs.append(f'{var2}*{num3}')
        inputs.append(f'{var2}?')
        expected.append(f'> {var2}*{num3}')
        expected.append(f'> {var2}?')
        expected.append(f'register {var2}: {num2*num3:,}')


    expected.append('> ')
    expected.append('ta daa!!!')
    expected.append('')

    write(inputs, f'cmake-build-{mode}/input.txt')
    write(expected, f'cmake-build-{mode}/expected.txt')


def exponetiation():
    vars = "abcdefghijklmnopqrstuvwxyz"
    inputs = []
    expected = []
    for i in range(500):
        var1, var2 = random.sample(vars, 2)
        num1 = random.randint(0, 9*10**20)
        num2 = random.randint(0, 20)

        inputs.append(f'{var1}={num1}')
        inputs.append(f'{var2}={num2}')
        expected.append(f'> {var1}={num1}')
        expected.append(f'> {var2}={num2}')

        inputs.append(f'{var1}^{var2}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}^{var2}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num1**num2:,}')

        num3 = random.randint(0, 10)
        num4 = random.randint(0, 10)
        
        inputs.append(f'{var1}={num3}')
        expected.append(f'> {var1}={num3}')

        inputs.append(f'{var1}^{num4}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}^{num4}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num3**num4:,}')


    expected.append('> ')
    expected.append('ta daa!!!')
    expected.append('')

    write(inputs, f'cmake-build-{mode}/input.txt')
    write(expected, f'cmake-build-{mode}/expected.txt')


def division():
    vars = "abcdefghijklmnopqrstuvwxyz"
    inputs = []
    expected = []
    for i in range(500):
        var1, var2 = random.sample(vars, 2)
        num1 = random.randint(0, 9*10**498)
        num2 = random.randint(1, 7*10**(int(0.1*i)))

        inputs.append(f'{var1}={num1}')
        inputs.append(f'{var2}={num2}')
        expected.append(f'> {var1}={num1}')
        expected.append(f'> {var2}={num2}')

        inputs.append(f'{var1}/{var2}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}/{var2}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num1//num2:,}')

    for i in range(400):
        var1 = random.choice(vars)
        num3 = random.randint(0, 9*10**499)
        num4 = random.randint(1, 9*10**497)
        
        inputs.append(f'{var1}={num3}')
        expected.append(f'> {var1}={num3}')

        inputs.append(f'{var1}/{num4}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}/{num4}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num3//num4:,}')

    for i in range(500):
        var1 = random.choice(vars)
        num3 = random.randint(0, 50000)
        num4 = random.randint(1, 100)
        
        inputs.append(f'{var1}={num3}')
        expected.append(f'> {var1}={num3}')

        inputs.append(f'{var1}/{num4}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}/{num4}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num3//num4:,}')

    for i in range(400):
        var1 = random.choice(vars)
        num3 = random.randint(0, 10**100)
        num4 = random.randint(1, 10**i)
        
        inputs.append(f'{var1}={num3}')
        expected.append(f'> {var1}={num3}')

        inputs.append(f'{var1}/{num4}')
        inputs.append(f'{var1}?')
        expected.append(f'> {var1}/{num4}')
        expected.append(f'> {var1}?')
        expected.append(f'register {var1}: {num3//num4:,}')


    expected.append('> ')
    expected.append('ta daa!!!')
    expected.append('')

    write(inputs, f'cmake-build-{mode}/input.txt')
    write(expected, f'cmake-build-{mode}/expected.txt')



def run_test(name):
    print(f'running {name} test')

    start = time.time()
    os.system(f"cmake-build-{mode}\\ass1.exe < \"cmake-build-{mode}/input.txt\" > \"cmake-build-{mode}/out.txt\"")
    end = time.time()
    print(f'took {end-start} seconds')

    outcome = filecmp.cmp(f'cmake-build-{mode}/expected.txt', f'cmake-build-{mode}/out.txt')

    if outcome:
        print(f' {bcolors.OKGREEN}test passed{bcolors.ENDC}')
    else:
        print(f' {bcolors.FAIL}test FAILED{bcolors.ENDC}')

    print()

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


addition()
run_test('addition')

multiplication()
run_test('multiplication')

exponetiation()
run_test('exponetation')

division()
run_test('division')