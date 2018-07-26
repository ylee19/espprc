class Edge
  attr_accessor :from, :to, :cost, :time_cost

  def initialize(from, to, cost, time_cost)
    @from, @to, @cost, @time_cost = from, to, cost, time_cost
  end

  def <=>(other)
    self.cost <=> other.cost
  end

  def to_s
    "#{from.to_s} => #{to.to_s} with cost #{cost}"
  end
end
