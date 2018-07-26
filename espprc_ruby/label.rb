class Label
	attr_accessor :node, :cost, :time_cost, :reachables

	def initialize(cost, time_cost, reachables, node)
		@cost = cost
		@time_cost = time_cost
		@reachables = reachables
		@node = node
	end

	def extend_label(from, to)
		edge = nil
		node.graph.edges.each do |e|
			if e.from == from && e.to == to
				edge = e
				break
			end
		end

		if @time_cost + edge.time_cost < to.due_time
			new_reachables = @reachables.clone
			new_reachables[to.name] = 1
			return Label.new(@cost + edge.cost, 
											@time_cost + edge.time_cost, 
											new_reachables,
											to)
		else
			return nil
		end
	end

	def equal?(label)
		@node.name == label.node.name &&
		@cost == label.cost &&
		@time_cost == label.time_cost &&
		@reachables == label.reachables
	end

	def dominate?(label)
		@cost <= label.cost &&
		@time_cost <= label.time_cost &&
		dominate_on_reachables?(label.reachables)
	end

	def to_s
		"[#{cost}, #{time_cost}, #{reachables}, #{node.name}]"
	end

	def dominate_on_reachables?(other_reachables)
		@reachables.each_pair do |key, value|
			return false if value > other_reachables[key]
		end
		return true
	end

end