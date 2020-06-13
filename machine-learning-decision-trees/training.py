import numpy as np
import time
import matplotlib.pyplot as plt
from utils import maximise_info_gain, split, bin_data, same_label
from evaluation import interpret_matrix, classification_rate, normalise_matrix
from visualisation import visualise_tree


CLEAN_DATASET_PATH = "wifi_db/clean_dataset.txt"
NOISY_DATASET_PATH = "wifi_db/noisy_dataset.txt"

clean_data = np.loadtxt(CLEAN_DATASET_PATH)
noisy_data = np.loadtxt(NOISY_DATASET_PATH)


def decision_tree_learning(training_dataset):
    if same_label(training_dataset):
        return {'att': None, 'aval': training_dataset[0][-1], 'depth': 0,
                'left': None, 'right': None}

    opt_split = maximise_info_gain(training_dataset)
    split_att = opt_split[0]
    split_val = opt_split[1].astype(float)

    left, right = split(training_dataset, training_dataset[:, split_att] < split_val)
    node = {'att': split_att, 'aval': split_val}
    node['left'], node['right'] = decision_tree_learning(left), decision_tree_learning(right)

    node['depth'] = max(node['left']['depth'], node['right']['depth']) + 1
    return node


def recalculate_depth(tree):
    # base case: if at a leaf
    if tree['att'] == None:
        tree['depth'] = 0
        return
    # recalculate depths for left and right trees
    recalculate_depth(tree['left'])
    recalculate_depth(tree['right'])
    tree['depth'] = max(tree['left']['depth'], tree['right']['depth']) + 1
    return


def pick_plum(tree, data, root):
    # save the state of the tree before pruning so we can revert if it isn't
    # worth pruning
    save_state = tree['att'], tree['aval']
    # calculate the accuracy pre-pruning
    acc = evaluate(root, data)
    # prune the tree and consider both left and right leaf values
    tree['att'] = None
    tree['aval'] = tree['left']['aval']
    l_acc = evaluate(root, data)
    tree['aval'] = tree['right']['aval']
    r_acc = evaluate(root, data)
    # decide which leaf value to inherit if it improved the accuracy
    if l_acc >= acc or r_acc >= acc:
        if l_acc > r_acc:
            # we don't need to do for the reverse (l_acc < r_acc) because its
            # already set to the right-side leaf 5 lines above
            tree['aval'] = tree['left']['aval']
        return True
    # otherwise revert back to the pre-pruned state
    else:
        tree['att'], tree['aval'] = save_state
        return False


def prune_tree(tree, data, root):
    if tree['att'] == None:
        return False
    elif tree['left']['att'] == None and tree['right']['att'] == None:
        return pick_plum(tree, data, root)
    else:
        plum = prune_tree(tree['left'], data, root)
        plum_ = prune_tree(tree['right'], data, root)
        return (plum or plum_)


def climb_tree(tree, data):
    # check if node is a leaf (i.e. split attribute is None)
    if tree['att'] == None:
        return tree['aval'].astype(int)
    # get the details of the split
    split_att = tree['att']
    split_val = tree['aval']
    # determine which side of the tree to traverse down
    if data[split_att] < split_val:
        return climb_tree(tree['left'], data)
    else:
        return climb_tree(tree['right'], data)


def get_conf_matrix(tree, data):
    confusion_matrix = np.zeros((4,4), int)
    for data_entry in data:
        actual = data_entry[-1].astype(int)
        predicted = climb_tree(tree, data_entry)
        confusion_matrix[actual-1][predicted-1] += 1
    return confusion_matrix


def evaluate(tree, data):
    conf = get_conf_matrix(tree, data)
    return classification_rate(conf)


def cross_validation(dataset):
    # create confusion_matrix
    confusion_matrix = np.zeros((4,4), int)
    # bin the dataset (into 10)
    bins = bin_data(dataset)
    # each bin takes turns becoming the validation set
    for v_index, v_bin in enumerate(bins):
        training = np.empty((0, v_bin.shape[1]))
        # and we stich the remaining 9 bins together
        for index, bin in enumerate(bins):
            if (index != v_index):
                training = np.vstack((training, bins[index]))
        # train the decision tree and sum the confusion matrices
        tree = decision_tree_learning(training)
        confusion_matrix += get_conf_matrix(tree, v_bin)
    return confusion_matrix


def cross_validation_with_pruning(training_data, test_data, pruning_bins):
    confusion_matrix = np.zeros((4, 4), int)
    bins = bin_data(training_data)

    for i in range(10):
        training_set = np.empty((0, test_data.shape[1]))
        validation_set = np.empty((0, test_data.shape[1]))
        for k in range(10):
            if (i+k)%10 < pruning_bins:
                validation_set = np.vstack((validation_set, bins[k]))
            else:
                training_set = np.vstack((training_set, bins[k]))

        tree = decision_tree_learning(training_set)
        while prune_tree(tree, validation_set, tree):
            pass
        recalculate_depth(tree)
        confusion_matrix += get_conf_matrix(tree, test_data)
    return confusion_matrix


def find_best_parameter(data):
    results = np.empty((810, 3))
    counter = 0
    bins = bin_data(data)
    for i in range(10):
        validation_set = bins[i]
        print("------------------------------")
        print("FOLD {}/{}".format(i+1, 10))
        print("------------------------------")
        for pb in range(0, 9):
            for k in range(9):
                tree_gen_set = np.empty((0, data.shape[1]))
                pruning_set = np.empty((0, data.shape[1]))
                for j in range(1, 10):
                    if k < j <= (pb+k)%9 or (pb+k)%9 < k < j or j <= (pb+k)%9 < k:
                        pruning_set = np.vstack((pruning_set, bins[(i+j)%10]))
                    else:
                        tree_gen_set = np.vstack((tree_gen_set, bins[(i+j)%10]))
                tree = decision_tree_learning(tree_gen_set)
                while pb > 0 and prune_tree(tree, pruning_set, tree):
                    pass
                recalculate_depth(tree)
                matrix = get_conf_matrix(tree, validation_set)

                results[counter][0] = pb
                results[counter][1] = tree['depth']
                results[counter][2] = classification_rate(matrix)
                counter += 1

            print("Completed assessment for pruning dataset size: {}".format(pb))
    print_best_parameter(results)


def print_best_parameter(results):
    totals = np.zeros((2, 9), float)
    for i in range(810):
        totals[0][results[i][0].astype(int)] += results[i][2]
        totals[1][results[i][0].astype(int)] += results[i][1]

    print()
    print("%Prune data\t", "Avg. Depth\t", "Avg. CR")
    avgs = np.zeros((2, 9), float)
    for i in range(9):
        avgs[0][i] = totals[0][i]/90
        avgs[1][i] = totals[1][i]/90
        print(np.around(i/0.09, 1), "\t\t",
              np.around(avgs[1][i], 1), "\t\t",
              np.around(avgs[0][i], 4))
    print()
    optimal_ratio = np.where(avgs[0]==max(avgs[0]))[0] / 0.09
    optimal_ratio = np.around(optimal_ratio, 1)
    print("Best ratio of data to use for validation (pruning) is: {}%".format(optimal_ratio))
