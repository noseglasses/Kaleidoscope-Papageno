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
      
      self.description(
"This test checks the correct behavior of Papageno's two key tapping.\n"
"\n"
"Two keys, the innermost thumb keys, trigger the Enter key when hit short \n"
"after another.\n"
"\n"
"This must work if the second key is hit while the first key is still \n"
"active (overlapping). \n"
"It also must work when the first key has already been released (sequencially).\n"
"Also after a timeout the keys must be output as if they were not affected\n"
"by Papageno (timeout).\n"
"Finally, when another unrelated key is pressed in between, pattern\n"
"recognition must be aborted and the tokens that have been queued so far must\n"
"be output, followed by the key that has triggered the interruption.\n"
"\n"
      )
      
      k1 = (3, 7) # Assigned to left shift through the keymap
      k2 = (3, 8) # Assigned to right shift through the keymap
      
      self.altKey = (0, 1) # Assigned to Key_1 through the keymap
            
      self.addPermanentReportAssertions([ 
         DumpReport()
      ])
            
      self.testOvelapping([k1, k2])
      self.testOvelapping([k2, k1])
      
      self.testSequencially([k1, k2])
      self.testSequencially([k2, k1])
      
      self.testTimeout([k1, k2])
      self.testTimeout([k2, k1])
      
      self.testInterruption(k1)
      self.testInterruption(k2)
      
   def testOvelapping(self, keys):
      
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
      
   def testSequencially(self, keys):
      
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
      
   def testTimeout(self, keys):
      
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
      self.skipTime(400, 
                    [NReportsGenerated(2)] # Make sure that there are 
                           # only two report generated for activating 
                           # and deactivating the flushed key on timeout
                   )

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
      self.skipTime(400, 
                    [NReportsGenerated(2)] # Make sure that there are 
                           # only two report generated for activating 
                           # and deactivating the flushed key on timeout
                  )
      
   def testInterruption(self, key):
      
      self.header("Checking Papageno two key tap alternatie key interruption")

      # The first key does not trigger any reports.
      #
      self.keyDown(*key)
      self.scanCycle([CycleHasNReports(0)])

      # Release the first key
      #
      self.keyUp(*key)
      self.scanCycle([CycleHasNReports(0)])
      
      # The following assertions are associated with three consequtive
      # keyboard reports.
      #
      self.queueReportAssertion(
         ReportModifiersActive([kaleidoscope.Layer.lookupOnActiveLayer(*key)], exclusively = True)
      )
      self.queueReportAssertion(ReportEmpty())
      self.queueReportAssertion(ReportKeyActive(kaleidoscope.Layer.lookupOnActiveLayer(*self.altKey)))
      
      self.keyDown(*self.altKey)
      self.scanCycle([CycleHasNReports(3)])
      
      self.queueReportAssertion(ReportEmpty())
      self.keyUp(*self.altKey)
      self.scanCycle([CycleHasNReports(1)])

def main():
    
   test = PapagenoTest()
   test.debug = True
      
   test.run()
   
   #test.graphicalMap()
   
   return test
                   
if __name__ == "__main__":
   global test
   test = main()
