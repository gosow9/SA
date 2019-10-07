# -*- coding: utf-8 -*-
"""
Week 3: build a linear regression model using only NumPy

06.05.2019 / Hannes Badertscher

"""
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import t


def simple_linear_regression(X, y):
    """ Performs a simple linear regression with a single input feature

    Implement the linear regression with numpy and manually estimates
    the standard errors, t-, and p-values and prints them to the console.

    Parameters
    ----------
    X : array_like
        The training data, given as a 1-D array
    y : array_like
        The labels for the data given in `X`, as a 1-D array.

    """

    # TODO: fit a linear regression using numpy

    # TODO: Print beta_0 and beta_1

    # TODO: calculate and print the standard errors of beta_0 and beta_1 using
    #       equation (3.8) from the book

    # TODO: calculate and print the t-values for beta_0 and beta_1

    # TODO: calculate and print the p-values for beta_0 and beta_1


def main(data_path):
    """ Main function which runs the linear regression on Advertising data """
    advertising_data = np.genfromtxt(data_path,
                                     delimiter=',',
                                     skip_header=True)

    sample = advertising_data[:, 0]
    voltage = advertising_data[:, 1]
    current = advertising_data[:, 2]
    power = voltage*current
    plt.plot(sample, voltage)
    plt.figure()
    plt.ylim(0,0.1)
    plt.grid(True)
    plt.plot(current[3000:4500],'.-')
#    plt.figure()
#    plt.plot(sample, power)
    print(np.max(current))
    #simple_linear_regression(tv_data, sales_data)


if __name__ == '__main__':
    main('./dlog2.csv')
