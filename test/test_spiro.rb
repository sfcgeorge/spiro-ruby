require 'minitest/autorun'
require 'spiro'

class TestSpiro < Minitest::Test
  def rand_n
    [
      Random.rand(-100..100),
      Random.rand(-100..100),
      [:node, :g2, :g4, :left, :right].sample
    ]
  end

  def test_spiros_to_splines_raises_on_bad_input
    assert_raises TypeError do
      Spiro.spiros_to_splines(nil, true)
    end
    assert_raises TypeError do
      Spiro.spiros_to_splines([1, 2, 3], true)
    end
    assert_raises TypeError do
      Spiro.spiros_to_splines([[1, 2, 3]], true)
    end
    assert_raises ArgumentError do
      Spiro.spiros_to_splines([[1, 2, :foo]], true)
    end
    assert_raises ArgumentError do
      Spiro.spiros_to_splines([[1, 2, :node, 4]], true)
    end
  end

  def test_spiros_to_splines_returns_array
    splines = Spiro.spiros_to_splines(
      [[0, 0, :node], [100, 50, :g2], [-50, 300, :node]],
      true
    )
    assert_kind_of Array, splines
  end

  #def test_spiros_to_splines_with_random_data_stress_test
    #while true do
      #path = Random.rand(5..50).times.map{rand_n}
      #p path
      #splines = Spiro.spiros_to_splines(path, true)
      #assert_includes [Array, NilClass], splines.class
    #end
  #end
end
