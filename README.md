# Collapstring

Collapse quoted strings in strings. Does not support JRuby.

## Why?

It's a simple, reliable and fast way to start making MySQL
requests shorter in a SQL logging pipeline.

## Installation

Add this line to your application's Gemfile:

    gem 'collapstring'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install collapstring

## Usage

    require 'collapstring'
    Collapstring.collapse 'SELECT "hello \"world\"";'
    Collapstring.collapse! 'SELECT "hello \"world\"";' # _could_ be in-place

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
