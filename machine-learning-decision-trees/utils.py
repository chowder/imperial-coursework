import math
import numpy as np
import time


def shannon(p):
    return p * math.log(p, 2)


"""
@return: the left and right splits of an array given a condition
"""
def split(arr, cond):
    return [arr[cond], arr[~cond]]


"""
@return: the shannon entropy of the dataset
"""
def entropy(dataset):
    labels = dataset[:, -1]
    unique, counts = np.unique(labels, return_counts=True)
    p_list = counts/len(labels)
    return -1 * sum(list(map(shannon, p_list)))


"""
@return: the remainder component of the infomation gain expression
"""
def remainder(left, right):
    H_left = entropy(left)
    H_right = entropy(right)
    coeff_denom = left.shape[0] + right.shape[0]
    return left.shape[0] / coeff_denom * H_left + \
           right.shape[0] / coeff_denom * H_right


"""
@return: the information gained from splitting the dataset by the given
         attribute/dimension and value
"""
def info_gain(dataset, split_att, split_val):
    # get entropy of total dataset
    H_all = entropy(dataset)
    # split left and right
    left, right = split(dataset, dataset[:, split_att] < split_val)
    # calculate remainder
    rem = remainder(left, right)
    return H_all - rem


"""
@return: for a given dataset, returns the optimal split in the format:-
         [<dimension to split>, <value to split>, <info gain from split>]
"""
def maximise_info_gain(dataset):
    max_info = 0
    split_val = None
    max_att = None

    for att in range(dataset.shape[1] - 1):
        dim = np.unique(dataset[:, att])
        midpoints = (dim[1:] + dim[:-1]) / 2
        max_att_info = 0
        split_att_val = None

        for val in midpoints:
            info = info_gain(dataset, att, val)
            if info > max_att_info:
                max_att_info = info
                split_att_val = val

        if max_att_info > max_info:
            max_info = max_att_info
            split_val = split_att_val
            max_att = att

    return max_att, split_val


"""
@return: 10 bins of equal row size
"""
def bin_data(dataset, bins=10):
    np.random.shuffle(dataset)
    return np.split(dataset, bins)


"""
@return: true if all entries in the dataset have the same label
         (value in the last column), false otherwise
"""
def same_label(dataset):
    labels = dataset[:, -1]
    uniques = np.unique(labels)
    return uniques.size == 1


"""
@return: the frequency of each dimension used as a split dimension in the given
         tree
"""
def dimension_frequency(tree):
    freq_array = np.zeros(7, int)
    return dimension_frequency_helper(tree, freq_array)


def dimension_frequency_helper(tree, freq_array):
    if tree['att'] == None:
        return
    freq_array[tree['att']] += 1
    dimension_frequency_helper(tree['left'], freq_array)
    dimension_frequency_helper(tree['right'], freq_array)
    return freq_array
