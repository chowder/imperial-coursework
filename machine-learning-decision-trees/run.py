import configparser
import numpy as np


from evaluation import interpret_matrix, normalise_matrix
from training import cross_validation, cross_validation_with_pruning, decision_tree_learning, prune_tree, recalculate_depth
from utils import bin_data, dimension_frequency
from visualisation import visualise_tree

if __name__ == "__main__":
    # First load config
    config = configparser.ConfigParser()
    config.read('config.ini')

    # Then load dataset
    dataset = np.loadtxt(config['Settings']['dataset'])

    # Section off one bin (200/2000) to be used as the test/final evaluation
    bins = bin_data(dataset)
    test_set = bins[0]
    # Stitch the rest of the bins back together
    training_data = np.empty((0, dataset.shape[1]))
    for i in bins[1:]:
        training_data = np.vstack((training_data, i))

    # get_best_parameter(training_data)

    # Do the thing
    print("--------------------------------------------------------------------")
    print("Performing simple cross validation on the dataset (without pruning):")
    print("--------------------------------------------------------------------")
    conf = cross_validation(dataset)
    print("\nRAW:")
    interpret_matrix(conf)

    print("\nNORMALISED:")
    normalised = normalise_matrix(conf)
    interpret_matrix(normalised)

    print("--------------------------------------------------------------------")
    print("Performing cross validation on the dataset (with pruning):")
    print("--------------------------------------------------------------------")
    conf = cross_validation_with_pruning(training_data, test_set, int(config['Settings']['pruning_validation_bins']))
    print("\nRAW:")
    interpret_matrix(conf)

    print("\nNORMALISED:")
    normalised = normalise_matrix(conf)
    interpret_matrix(normalised)

    # Print the trees
    if config['Settings']['display_tree']:

        bins = bin_data(dataset)
        training_data = np.empty((0, dataset.shape[1]))
        validation_data = np.empty((0, dataset.shape[1]))

        for i in bins[:int(config['Settings']['pruning_validation_bins'])]:
            training_data = np.vstack((training_data, i))
        for i in bins[int(config['Settings']['pruning_validation_bins']):]:
            validation_data = np.vstack((validation_data, i))

        # create and display unpruned_tree
        tree = decision_tree_learning(training_data)
        print("The UNPRUNED tree has a maximal depth of {}".format(tree['depth']))
        print("Frequency of each dimension: {}".format(dimension_frequency(tree)))
        visualise_tree(tree)

        # prune the tree and display it
        while prune_tree(tree, validation_data, tree):
            pass
        recalculate_depth(tree)
        print("The PRUNED tree has a maximal depth of {}".format(tree['depth']))
        print("Frequency of each dimension: {}".format(dimension_frequency(tree)))
        visualise_tree(tree)
