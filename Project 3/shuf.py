import argparse, random, sys, string

class shuf:
    def __init__(self, input, head_count, repeat):
        self.input = input
        self.head_count = head_count
        self.repeat = repeat

        #shuffle input lines
        random.shuffle(self.input)

    def output_shuffle(self):
        #if input is empty, nothing to shuffle
        if not len(self.input):
            return
        #not unique output
        if self.repeat:
            while self.head_count > 0:
                sys.stdout.write(random.choice(self.input))
                self.head_count = self.head_count - 1
        #unique output
        else:
            for i in range(0, self.head_count):
                sys.stdout.write(self.input[i]) 

def main():
    usage_msg = """Usage: %prog [OPTION]... [FILE]\n\
  or:  %prog -r [OPTION]... [ARG]...\n\
  or:  %prog -i LO-HI [OPTION]...\n\

Write a random permutation of the input lines to standard output"""

    parser = argparse.ArgumentParser(prog='shuf.py',
                          usage=usage_msg)
    parser.add_argument("-i", "--input-range",
                      action="store", type=str, dest="input_range", default="",
                      help="treat each number LO through HI as an input line")
    parser.add_argument("-n", "--head-count",
                      action="store", type=int, dest="head_count", default=sys.maxsize,
                      help="output at most COUNT lines")
    parser.add_argument("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")
    #check for positional arguments
    parser.add_argument('filename', type=str, nargs='*')
    options = parser.parse_args()
    repeat = bool(options.repeat)
    filename = options.filename 
    #check is there is more than 1 positional argument
    if len(filename) > 1:
        tempStr = " ".join(filename[1:])
        parser.exit(1, "extra positional arguments '{0}'\n".format(tempStr))
    
    head_count = options.head_count
    #check if head_count is negative
    if head_count < 0:
        parser.exit(1, "negative count: {0}\n".
                     format(head_count))

    input_range = options.input_range
    #if input_range is passed in, else use the file or standard input
    if len(input_range) > 0:
        #check for dash
        try:
            dash = input_range.index('-')
        except ValueError as e:
            parser.exit(1, "invalid input range: '{0}'\n".
            format(options.input_range))
        if dash == 0:
            parser.exit(1, "invalid input range: '{0}'\n".
            format(options.input_range))

        begin, end = input_range.split("-")
        
        #check for valid numbers
        try:
            range_begin = int(begin)
        except ValueError as e:
            parser.exit(1, "invalid input range: '{0}'\n".
            format(options.input_range))
        
        try:
            range_end = int(end)
        except ValueError as e:
            parser.exit(1, "invalid input range: '{0}'\n".
            format(options.input_range))
        
        if range_begin > range_end:
            parser.exit(1, "invalid input range, low needs to be lower than high: '{0}'\n".
            format(options.input_range))
        
        #create list of inputs for the -i option
        input = list(range(range_begin,range_end+1))
        for i in range(len(input)):
            input[i] = str(input[i]) + "\n"
    else:
        #checking input from standard input
        if (len(filename) == 0 or filename[0] == "-"):
                input = sys.stdin.readlines()
        
        else:
            #checking input from a file
            try:
                f = open(filename[0], 'r')
                input = f.readlines()
                f.close()
            except IOError as e:
                errno, strerror = e.args
                parser.exit(1, "file error({0}): {1}\n".format(errno,strerror))
    
    #checking if head_count is greater than the input
    if head_count > len(input) and not repeat:
        head_count = len(input)

    shuffle_text = shuf(input, head_count, repeat)
    shuffle_text.output_shuffle()


if __name__ == "__main__":
    main()
