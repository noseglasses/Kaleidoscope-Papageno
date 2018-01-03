#!/usr/bin/python3

# -*- mode: c++ -*-
# Kaleidoscope-Python -- Wraps Kaleidoscope modules' c++
#    code to be available in Python programs.
# Copyright (C) 2017 noseglasses <shinynoseglasses@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This is a python based implementation of the Kaleidoscope-Dual-Use plugin
#
# It demonstrates how plugins can be prototyped with Kaleidoscope-Python
# before finally implementing them as C++ code.

import kaleidoscope
from kaleidoscope import *

import sys

class PapagenoTest(Test):

   def run(self):
      
      k1 = (3, 7)
      k2 = (3, 8)
            
      self.addPermanentReportAssertions([ 
         DumpReport()
      ])
            
      #self.testTwoKeyTappingOverlapping([k1, k2])
      #self.testTwoKeyTappingOverlapping([k2, k1])
      
      #self.testTwoKeyTappingSequencially([k1, k2])
      #self.testTwoKeyTappingSequencially([k2, k1])
      
      #self.testTwoKeyTappingTimeout([k1, k2])
      self.testTwoKeyTappingTimeout([k2, k1])
      
   def testTwoKeyTappingOverlapping(self, keys):
      
      self.header("Checking Papageno overlapping two key tap")
      
      # The first key does not trigger any reports.
      #
      self.keyDown(*keys[0])
      self.scanCycle([CycleHasNReports(0)])

      # The second key, together with the first triggers Enter.
      #
      self.keyDown(*keys[1])
      
      # The next key report must only contain the Enter key
      #
      self.queueGroupedReportAssertions([ 
         ReportKeysActive([keyEnter()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.scanCycle([CycleHasNReports(1)])

      # Release the first key
      #
      self.keyUp(*keys[0])
      
      # Releasing the key also releases the Enter key that was the 
      # action of the two key tapping pattern.
      #
      self.queueGroupedReportAssertions([  
         ReportEmpty()
      ])
      self.scanCycle([CycleHasNReports(1)])

      # Release the second key
      #
      self.keyUp(*keys[1])    

      # The second key release must not trigger a keyboard report.
      #
      self.scanCycles(2, cycleAssertionList = [CycleHasNReports(0)])
      
   def testTwoKeyTappingSequencially(self, keys):
      
      self.header("Checking Papageno sequencial two key tap")
      
      # The first key does not trigger any reports.
      #
      self.keyDown(*keys[0])
      self.scanCycle([CycleHasNReports(0)])

      # Release the first key
      #
      self.keyUp(*keys[0])
      self.scanCycle([CycleHasNReports(0)])
      
      # The second key, together with the first triggers Enter.
      #
      self.keyDown(*keys[1])
      
      # The next key report must only contain the Enter key
      #
      self.queueGroupedReportAssertions([ 
         ReportKeysActive([keyEnter()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.scanCycle([CycleHasNReports(1)])

      # Release the second key
      #   
      self.queueGroupedReportAssertions([  
         ReportEmpty()
      ])
      self.keyUp(*keys[1])
      self.scanCycle([CycleHasNReports(1)]) 

      # The second key release must not trigger a keyboard report.
      #
      self.scanCycles(2, cycleAssertionList = [CycleHasNReports(0)])
      
   def testTwoKeyTappingTimeout(self, keys):
      
      self.header("Checking Papageno two key tap timeout")

      # The first key does not trigger any reports.
      #
      self.keyDown(*keys[0])
      self.scanCycle([CycleHasNReports(0)])

      # Release the first key
      #
      self.keyUp(*keys[0])
      self.scanCycle([CycleHasNReports(0)])
      
      # The next report (on timeout) must contain the modifiers (r/l shift)
      #
      self.queueGroupedReportAssertions([ 
         ReportModifiersActive([kaleidoscope.Layer.lookupOnActiveLayer(*keys[0])], exclusively = True)
      ])
      
      # Wait for more than 200 millis
      #
      self.skipTime(400)

      self.keyDown(*keys[1])
      self.scanCycle([CycleHasNReports(0)])

      # Release the second key
      #   
      self.keyUp(*keys[1])
      self.scanCycle([CycleHasNReports(0)])
         
      self.queueGroupedReportAssertions([ 
         ReportModifiersActive([kaleidoscope.Layer.lookupOnActiveLayer(*keys[1])], exclusively = True)
      ])
            
      # Wait for more than 200 millis
      #
      self.skipTime(400)

def main():
    
   test = PapagenoTest()
   test.debug = True
      
   test.run()
   
   #test.graphicalMap()
   
   return test
                   
if __name__ == "__main__":
   global test
   test = main()
