class LabelCorrecting

	def initialize(graph, source_node)
    @graph = graph
    @source_node = source_node
    @graph.nodes.each do |node|
    	if node.name == source_node.name
    		reachables = {}
    		@graph.nodes.each do |node|
    			if node == source_node
    				reachables[node.name] = 1
    			else
    				reachables[node.name] = 0
    			end
    		end
    		label = Label.new(0, 0, reachables, source_node)
    		node.labels = [label]
    	else
    		node.labels = nil
    	end
    end
    @to_treat = [source_node]
  end

  def calculate
  	while @to_treat.any? do     
  		@to_treat.each do |node|
  			node.successors.each do |succ|
          t_labels = succ.labels.clone
  				labels = []
          puts "######################{succ.name}#####################"
  				node.labels.each do |label|
  					if label.reachables[succ.name] == 0
  						new_label = label.extend_label(node, succ)
  						labels << new_label if new_label
  					end
  				end

          if succ.labels.nil?
            succ.labels = labels
          else
  				  succ.labels = succ.labels + labels
          end

          puts "######before########"
          puts succ.labels
          puts "#######after########"
  				succ.labels = succ.remove_dominated_labels
          puts succ.labels
          puts "#####################"

          # to check if the labels of the successors has changed
  				if t_labels != succ.labels
  					@to_treat << succ
            @to_treat.uniq!
  				end
  			end


	  		@to_treat.each_with_index do |n, i|
	  			@to_treat.delete_at(i) if node == n
	  		end
        puts "**********************"
        puts @to_treat.join(", ")
        puts "**********************"
        puts "##############################################\n\n"
        sleep 1
  		end
  	end
  end
end