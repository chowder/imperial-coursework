import numpy as np

def precision(conf):
    p_list = []
    for dim in range(conf.shape[0]):
        true_positive = conf[dim][dim]
        predicted_positive = conf.sum(axis=0)[dim]
        p_val = true_positive / predicted_positive
        p_list.append(p_val)
    return p_list


def recall(conf):
    r_list = []
    for dim in range(conf.shape[0]):
        true_positive = conf[dim][dim]
        total_positive = conf[dim].sum()
        r_val = true_positive / total_positive
        r_list.append(r_val)
    return r_list


def classification_rate(conf):
    sum_conf = conf.sum()
    sum_diag = np.diagonal(conf).sum()
    cr_val = sum_diag/sum_conf
    return cr_val


def f_measure(recall, precision):
    f_m_list = []
    for r, p in zip(recall, precision):
        f_m_list.append(2 * r * p / (r + p))
    return f_m_list


def interpret_matrix(conf):
    cr = classification_rate(conf)
    r = recall(conf)
    p = precision(conf)
    f_m = f_measure(r, p)
    print("Matrix:")
    print(conf)
    print("Classification rate:", np.around(cr, 3))
    print("Recall:", np.around(r, 3))
    print("Precision:", np.around(p, 3))
    print("F-Measure:", np.around(f_m, 3))


def normalise_matrix(matrix):
    normalised_matrix = np.zeros((4, 4), float)
    for i in range(matrix.shape[0]):
        row_sum = matrix[i].sum()
        for j in range(matrix.shape[1]):
            normalised_matrix[i][j] = matrix[i][j]/row_sum
    return normalised_matrix
