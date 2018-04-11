#!/usr/bin/python3

# -*- mode: c++ -*-
# Kaleidoscope-Papageno -- Papageno features for Kaleidoscope
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

import leidokos
from leidokos import *

class NoseglassesTest(TestDriver):

   def keyTap(self, key):
            
      self.keyDown(*key)
      self.scanCycle()
      self.keyUp(*key)
      self.scanCycle()

   def run(self):
      
      self.description(
"This tests noseglasses' firmware sketch.\n"
      )
      
      leftThumb1 = (0, 7)
      leftThumb2 = (1, 7)
      leftThumb3 = (2, 7)
      leftThumb4 = (3, 7)
      
      rightThumb1 = (3, 8)
      rightThumb2 = (2, 8)
      rightThumb3 = (1, 8)
      rightThumb4 = (0, 8)
      
      special1 = (0, 13)
      special2 = (0, 15)
      special3 = (0,  0)
      special4 = (0,  1)
      special5 = (0,  2)
      special6 = (0,  3)
      
      ng_Key_E = (2,  3)
      ng_Key_O = (2, 13)
      ng_Key_I = (2, 12)
      ng_Key_S = (2,  2)
      
      ng_Key_A = (2,  1)
            
      self.addPermanentReportAssertions([ 
         DumpReport()
      ])
            
      self.header("A cluster that causes enter (key order arbitrary)")
      #
      # {LeftThumb3, RightThumb2} : Key_Enter
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyEnter()], exclusively = True)
      ])
      self.keyTap(leftThumb3)
      self.keyTap(rightThumb2)
      self.checkStatus()

      self.header("Double tap on the left inner thumb key triggers a user function")
      #
      # |LeftThumb3|*2 : doubleTab
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyTab()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyTab()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(leftThumb3)
      self.keyTap(leftThumb3)
      self.checkStatus()
      
      self.header("A note line with two thumb keys triggers tab")
      #
      # |LeftThumb3| -> |RightThumb3| : Key_Tab
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyTab()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(leftThumb3)
      self.keyTap(rightThumb3)
      self.checkStatus()

      self.header("Double tap on right inner thumb key")
      #
      # |RightThumb2|*2 : aShiftTab
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyTab()]),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(rightThumb2)
      self.keyTap(rightThumb2)
      self.checkStatus()
      
      self.header("Note line for key delete")
      #
      # |RightThumb2| -> |LeftThumb2| : Key_Delete
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyDelete()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(rightThumb2)
      self.keyTap(leftThumb2)
      self.checkStatus()

      self.header("|Special1|*2 : repeatLastCommand")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyUpArrow()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyEnter()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special1)
      self.keyTap(special1)
      self.checkStatus()
      
      # Search commands
      #
      # |Special3|*3 : ordinarySearch@2, fileSearch@3
      #
      self.header("Ordinary search")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyF()], exclusively = True),
         ReportModifiersActive([keyLCtrl()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyEnter()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special3)
      self.keyTap(special3)
      self.skipTime(500) # Enable timeout
      self.checkStatus()
      
      self.header("File search")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyF1()], exclusively = True),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyEnter()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special3)
      self.keyTap(special3)
      self.keyTap(special3)
      self.checkStatus()

      self.header("Search with ctrl-F")
      #
      # |Special4|*2 : leftCTRL_F
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyF()], exclusively = True),
         ReportModifiersActive([keyLCtrl()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special4)
      self.keyTap(special4)
      self.checkStatus()
      
      self.header("Hit F3")
      #
      # |Special5|*2 : Key_F3
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyF3()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special5)
      self.keyTap(special5)
      self.checkStatus()
      
      self.header("Shift-Ctrl-C")
      #
      # |Special6|*2 : shiftCtrlC
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyC()], exclusively = True),
         ReportModifiersActive([keyLCtrl(), keyLShift()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(special6)
      self.keyTap(special6)
      self.checkStatus()

      # Assign german umlauts as tripple taps to
      # suitable and non-colliding (digraphs!) keys of the home row
      #
      # Occurence probabilities of umlauts in written german:
      #
      # a-umlaut  0,54#
      # o-umlaut  0,30#
      # u-umlaut  0,65#
      # s-umlaut  0,37#
      #
      # These are used together with the EURKEY keyboard layout
      # that is available on Linux and Windows
      #
      self.header("|NG_Key_E|*3 : umlaut_A")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(ng_Key_E)
      self.keyTap(ng_Key_E)
      self.keyTap(ng_Key_E)
      self.checkStatus()
      
      self.header("|NG_Key_O|*3 : umlaut_O")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyO()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(ng_Key_O)
      self.keyTap(ng_Key_O)
      self.keyTap(ng_Key_O)
      self.checkStatus()
      
      self.header("|NG_Key_I|*3 : umlaut_U")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyU()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(ng_Key_I)
      self.keyTap(ng_Key_I)
      self.keyTap(ng_Key_I)
      self.checkStatus()
      
      self.header("|NG_Key_S|*3 : umlaut_S")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(ng_Key_S)
      self.keyTap(ng_Key_S)
      self.keyTap(ng_Key_S)
      self.checkStatus()
      
      # Check some corner cases
      #
      self.header("M1 and a - 1")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyBackslash()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyDown(*rightThumb2)
      self.scanCycle()
      self.keyDown(*ng_Key_A)
      self.scanCycle()
      self.keyUp(*ng_Key_A)
      self.keyUp(*rightThumb2)
      self.scanCycle()
      self.checkStatus()
      
      self.header("M1 and a - 2")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyBackslash()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyDown(*rightThumb2)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*rightThumb2)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyDown(*ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.checkStatus()
      

def main():
    
   test = NoseglassesTest()
   test.debug = True
      
   test.run()
   
   return test
                   
if __name__ == "__main__":
   global test
   test = main()
