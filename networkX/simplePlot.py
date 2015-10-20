import networkx as nx
import matplotlib.pyplot as plt
import sys 

def draw_graph(graph):
    # extract nodes from graph
    nodes = set([n1 for n1, n2 in graph] + [n2 for n1, n2 in graph])
    # create networkx graph
    G=nx.DiGraph()
    # add nodes
    for node in nodes:
        G.add_node(node)
    # add edges
    for edge in graph:
        G.add_edge(edge[0], edge[1])
    # draw graph
    pos = nx.shell_layout(G)
    nx.draw(G, pos)
    # show graph
    plt.show()
# draw example

edges = []
graph = sys.argv[1].split('|')
for x in graph:
    x = x[1:-1]
    if x :
        edges.append(x.split(','))

draw_graph(edges)
