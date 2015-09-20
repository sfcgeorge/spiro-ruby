# Spiro

A Ruby wrapper for [libspiro](https://github.com/fontforge/libspiro), which is itself a bundling of [Spiro](http://levien.com/spiro/) by Raph Levien.

Spiro makes drawing beautiful curves easy.

Usually Bézier splines are used for curves. These require off-curve points which "bend" the line towards them. As these points aren't on the line they are quite unintuitive and it's hard to imagine where the line will end up without a GUI.

Raph Levien's Spiro splines only use on-curve points and so are easier to use and more intuitive to the artist. They make it easy to maintain constant curvature as well as constant slope. Such curves will simply look nicer.

This library will take an array of Spiro control points and convert them into a series of Bézier splines which can then be used in the myriad of ways the world has come to use Béziers.

It was created for the design as code tool [Vector Salad](https://github.com/sfcgeorge/vector_salad).


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'spiro'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install spiro


## Usage

```ruby
# takes an array of spiro points and returns an array of bezier points
# (this will potentially contain both cubic and quadratic beziers)
splines = Spiro.spiros_to_splines(
  [[0, 0, :node], [100, 50, :g2], [-50, 300, :node]],
  true # the path is closed
)
```

### Node types:

* :node, a standard non curved point
* :g2, a node with g2 curvature
* :g4, a node with g4 curvature
* :left, used to connect a curved line to a straight one
* :right, used to connect a straight line to a curved one. If you have a contour which is drawn clockwise, and you have a straight segment at the top, then the left point of that straight segment should be a left constraint, and the right point should be a right constraint.

It's often best to just experiment with the above until you get the desired result.

If it fails to produce a curve, then try switching between g2 and g4 points, and generally experimenting with moving nodes around.


## Contributing

1. Fork it ( https://github.com/[my-github-username]/spiro/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
