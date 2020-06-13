import matplotlib.pyplot as plt

def visualise_tree(tree):
    graph = visualise_tree_helper(tree, tree['depth'])
    graph.axis('off')
    graph.show()
    return

def visualise_tree_helper(tree, depth, position=[0,0]):
    bbox_props = dict(boxstyle="round", fc="w", ec="0.5")
    # base case: if at a leaf
    if tree['att'] == None:
        annotation = int(tree['aval'])
        plt.text(position[0], position[1], annotation, ha="center", va="center",
                 bbox=bbox_props)
        return
    # the height of the new depth
    height = position[1] - (1.4142 ** (depth))
    # calculate x (horizontal) positions for each child
    left_x = position[0] - (2 ** depth)
    right_x = position[0] + (2 ** depth)
    # plot the child branches
    plt.plot([left_x, position[0], right_x], [height, position[1], height])
    # annotate the parent branch on graph
    annotation = "x{} < {}".format(tree['att'], tree['aval'])
    plt.text(position[0], position[1], annotation, ha="center", va="center",
             bbox=bbox_props)
    # recurse for each child of the branch
    visualise_tree_helper(tree['left'], depth - 1, [left_x, height])
    visualise_tree_helper(tree['right'], depth - 1, [right_x, height])
    return plt
