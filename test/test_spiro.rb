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
    true)
    assert_kind_of Array, splines
  end

  #def test_spiros_to_splines_with_random_data_stress_test
    ## It will crash after a while, see below
    #while true do
      #path = Random.rand(5..50).times.map{rand_n}
      #p path
      #splines = Spiro.spiros_to_splines(path, true)
      #assert_includes [Array, NilClass], splines.class
    #end
  #end

  def test_this_one_crashes
    # I sometimes get "pointer being freed was not allocated" but it takes a while.
    # I thought it might be some nodes that Spiro doesn't like, but these are fine
    # sometimes, it's only after running several times that it crashes.
    # There must be a bad bit of memory management somewhere :-/
    #
    # Other times I get "Segmentation fault".
    #
    # It can crash very quickly, or it can take several seconds. So I'm guessing
    # that there a piece of memory is being freed when it shouldn't somewhere,
    # and it depends where that memory is whether it hurts anything. Not good.
    #
    # Anyway, I don't really know C and because all of the Ruby stacktrace
    # thrown in I can't figure out if the problem is in my code or Spiro.
    # Does anyone know C well enough to fix this?
    path = [[-39, -57, :g2], [-59, 20, :right], [37, -14, :g2], [-67, -40, :g2], [-83, 20, :g2], [56, -87, :left], [-11, 86, :left], [11, 29, :left], [-87, 54, :left]]
    while true do
      splines = Spiro.spiros_to_splines(path, true)
      assert_includes [Array, NilClass], splines.class
    end
  end
end
