def sign(number):
    if number < 0:
        return -1
    elif number > 0:
        return 1
    else:
        return 0

def calc(H,l,x):
    '''
    :param H: height of test object
    :param l: zigzag of test object
    :param x: distance from central to camera
    :return: pixel number of linear camera
    Если функция принимает отрицательное значение x, то принимается решение, что передаются координаты
    леовй камеры; Если функции передаётся отрицательное значение l, то считается, что объект обнаружения
    расположен слева от центральной камеры.
    '''
    F = 75 # focus
    h_cal = 6000  # calibration height
    size_pix = 0.008  # size of pixel

    if (x < 0 and l < 0) or (x > 0 and l > 0):
         tg_alpha_beta = abs(H/(x - l))
         tg_alpha = abs(h_cal/x)
    elif x == 0:
        return 1800 - int((F*l/H)/size_pix)

    else:
        tg_alpha_beta = abs(h_cal / x)
        tg_alpha = abs(H / (x - l))

    tg_beta = (tg_alpha_beta - tg_alpha)/(tg_alpha_beta*tg_alpha - 1)
    num_pix = int(tg_beta*F/size_pix)
    if sign(l) == sign(x):
        return 1800 - num_pix
    else:
        return 1800 + num_pix
if __name__ == '__main__':
    H = list(range(5400, 7000, 200))
    l = list(range(-500,550,50))
    with open('data.txt','w') as f:
        for height in H:
            f.write('\nH = {0}\nl\t'.format(height))
            for zigzag in l:
                f.write('{}\t'.format(zigzag))
            f.write('\nleft\t')

            for zigzag in l:
                f.write('{},'.format(calc(height,zigzag,-700)))
            f.write('\ncent\t')

            for zigzag in l:
                f.write('{},'.format(calc(height,zigzag,0)))
            f.write('\nright\t')

            for zigzag in l:
                f.write('{},'.format(calc(height,zigzag,700)))
