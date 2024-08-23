#!/usr/bin/env python

"""
Erik Soto
CMPS 3500 
Lab 13: Python Functions and Coroutines
"""

from sys import argv

# returns the result of applying  func to each element in list
def my_map(func, list):
  new_list = []
  for item in list:
    new_list.append(func(item))
  return new_list

# print my_map(lambda x: x + 1, [1, 2, 3])  # returns [2, 3, 4]

def my_filter(func, list):
  new_list = []
  for item in list:
    if func(item):
      new_list.append(item)
  return new_list

def my_mean(func, list):
  count=0
  sum=0.0
  for item in list:
    if func(item):
      count = count + 1
      sum = sum+item
  return sum/count 


import sys

def printjob(entry):  
   n,x = entry
   buf = n + "," + str(x) + " "
   sys.stdout.write(buf) 

"""
*  CONSUMER 
*  'yield stuff' passes stuff back to the producer; when control resumes a 
*  message (it may be empty) is available from the producer as the return 
*  value from yield
"""
def consumer(jobs):

    sys.stdout.write("Consumer starting.\n")
    i = -1 

    # as long as there is something in the jobs list keep processing requests
    while jobs:
        # yield passes control back to producer with the i job name
        i = (i + 1) % len(jobs)     # circular traversal of the list
        n,t = jobs[i]               # unpack job into name and jobno
        getRequest = yield jobs[i]  # wait for request and return name

        if getRequest:              # if getRequest is not empty process it
            request, name, jobno = getRequest

            if request == "add":
                entry = name,jobno   # pack job entry
                jobs.append(entry)
                sys.stdout.write("\nADD ") 

            elif request == "remove":
                entry = name,jobno
                if entry in jobs:
                    jobs.remove(entry)
                    buf = "\nREMOVE " + name + "\n"
                    sys.stdout.write(buf)
# END CONSUMER

"""
+  PRODUCER 
+  starts consumer and sends consumer jobs 
+  next passes a job to the consumer with no message passing
+  send passes a job to the consumer with a message 
"""

def producer(jobs):

   sys.stdout.write("Producer starting.\n")
   c = consumer(jobs)                  # start the consumer s 
  
   for i in range(len(jobs)): 
      printjob(c.next())               # s.next sends job to consumer w/ no msg 

   printjob(c.send(("add","iron",44)))   # s.send sends job to consumer w/ msg
   sys.stdout.write("\n")
   for i in range(len(jobs)): 
      printjob(c.next())               

   c.send(("remove","iron",44))
   for i in range(len(jobs)): 
      printjob(c.next())

   print ("\nProducer Done.") 
# end PRODUCER


# PART II MAIN
if len(argv) < 2:
  print ("Usage: %s <filename>" % argv[0])
  sys.exit(0)

input = file(argv[1])
values = []
for line in input:
   values.append(int(line))

filtered = my_filter(lambda x: (x % 2) != 0, values)
mean = my_mean(lambda x: x > 0, values)
squared = my_map(lambda x: x * x, values)

print ("PART I") 
print ("original list: ", values) 
print ("odds:   " , filtered) 
print ("squared:" , squared)
print ("mean:   " , mean) 

# PART II
print ("PART II")
# initialize job list and start the producer coroutine
offset = ""
n = 0

if len(sys.argv) == 3:
   offset = sys.argv[2]
   n = int(offset)
    
if __name__ == "__main__":             # this means initialize once only

   jobs = [("wash",11+n),("dry",22+n),("fold",33+n)]
   producer(jobs)      # start producer                

# end MAIN
