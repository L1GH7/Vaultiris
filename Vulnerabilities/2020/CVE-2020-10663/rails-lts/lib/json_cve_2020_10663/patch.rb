require 'json'

module JSON
  class << self
    alias :parse_with_bad_defaults :parse

    def parse(string, *args)
      if args.size >= 1 && args[0].nil?
        args = []
      end

      parse_with_bad_defaults(string, *args)
    end
  end
end

