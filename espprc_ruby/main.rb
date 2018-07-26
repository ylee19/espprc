require_relative 'graph'
require_relative 'node'
require_relative 'edge'
require_relative 'label'
require_relative 'label_correcting.rb'

@graph = Graph.new

@graph.add_node(@node_p = Node.new("P", 0, 1000, 0))
@graph.add_node(@node_v2 = Node.new("V2", 0, 1000, 0))
@graph.add_node(@node_v3 = Node.new("V3", 0, 1000, 0))
@graph.add_node(@node_d = Node.new("D", 0, 1000, 0))

@graph.add_edge(@node_p, @node_v2, 2, 2)
@graph.add_edge(@node_p, @node_v3, 2, 2)
@graph.add_edge(@node_v2, @node_v3, 1, -2)
@graph.add_edge(@node_v3, @node_v2, 1, 1)
@graph.add_edge(@node_v2, @node_d, 2, 2)
@graph.add_edge(@node_v3, @node_d, 2, 2)

label_correcting = LabelCorrecting.new(@graph, @node_p)
label_correcting.calculate

@graph.nodes.each do |node|
	puts node
	puts node.labels
end