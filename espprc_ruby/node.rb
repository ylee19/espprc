class Node
  attr_accessor :name, :graph, :ready_time, :due_time, :servie_time, :labels

  def initialize(name, ready_time, due_time, servie_time)
    @name, @ready_time, @due_time, @servie_time = name, ready_time, due_time, servie_time
  end

  def adjacent_edges
    graph.edges.select{|e| e.from == self}
  end

  def successors
    succ = []
    graph.edges.each do |edge|
      if edge.from == self
        succ << edge.to
      end
    end
    succ.uniq
  end

  def to_s
    @name
  end

  def remove_dominated_labels
    new_labels = []
    labels.uniq! {|label| label.to_s}

    if labels.size == 1
      new_labels = labels
    else
      labels.size.times do |i|
        label_i = labels[i]
        t_labels = labels.reject {|label| label == label_i }

        if t_labels.all? {|label_j| !label_j.dominate?(label_i) }
          new_labels << label_i
        end
      end
      labels = new_labels
    end
  end

end