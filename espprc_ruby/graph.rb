class Graph
  attr_accessor :nodes
  attr_accessor :edges

  def initialize
    @nodes = []
    @edges = []
  end

  def add_node(node)
    nodes << node
    node.graph = self
  end

  def add_edge(from, to, cost, time_cost)
    edges << Edge.new(from, to, cost, time_cost)
  end

  def nb_nodes
    @nodes.count
  end

end
