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
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*key)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()

   def run(self):
      
      self.description(
"This tests noseglasses' firmware sketch.\n"
      )
      
      self.leftThumb1 = (0, 7)
      self.leftThumb2 = (1, 7)
      self.leftThumb3 = (2, 7)
      self.leftThumb4 = (3, 7)
      
      self.rightThumb1 = (3, 8)
      self.rightThumb2 = (2, 8)
      self.rightThumb3 = (1, 8)
      self.rightThumb4 = (0, 8)
      
      self.special1 = (0, 13)
      self.special2 = (0, 15)
      self.special3 = (0,  0)
      self.special4 = (0,  1)
      self.special5 = (0,  2)
      self.special6 = (0,  3)
      
      self.ng_Key_E = (2,  3)
      self.ng_Key_O = (2, 13)
      self.ng_Key_I = (2, 12)
      self.ng_Key_S = (2,  2)
      
      self.ng_Key_A = (2,  1)
      self.ng_Key_C = (3,  3)
            
      self.errorIfReportWithoutQueuedAssertions = True
      
      self.addPermanentReportAssertions([ 
         DumpReport()
      ])
      
      self.runTestSeries()
      
   def test1(self):
            
      self.header("A cluster that causes enter (key order arbitrary)")
      #
      # {LeftThumb3, RightThumb2} : Key_Enter
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyEnter()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.leftThumb3)
      self.keyTap(self.rightThumb2)
      self.checkStatus()

   def test2(self):
      
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
      self.keyTap(self.leftThumb3)
      self.keyTap(self.leftThumb3)
      self.checkStatus()
      
   def test3(self):
      
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
      self.keyTap(self.leftThumb3)
      self.keyTap(self.rightThumb3)
      self.checkStatus()

   def test4(self):
      
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
      self.keyTap(self.rightThumb2)
      self.keyTap(self.rightThumb2)
      self.checkStatus()
      
   def test5(self):
      
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
      self.keyTap(self.rightThumb2)
      self.keyTap(self.leftThumb2)
      self.checkStatus()

   def test6(self):
      
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
      self.keyTap(self.special1)
      self.keyTap(self.special1)
      self.checkStatus()
      
   def test7(self):
      
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
      self.keyTap(self.special3)
      self.keyTap(self.special3)
      self.skipTime(500) # Enable timeout
      self.checkStatus()
      
   def test8(self):
      
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
      self.keyTap(self.special3)
      self.keyTap(self.special3)
      self.keyTap(self.special3)
      self.checkStatus()

   def test9(self):
      
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
      self.keyTap(self.special4)
      self.keyTap(self.special4)
      self.checkStatus()
      
   def test10(self):
      
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
      self.keyTap(self.special5)
      self.keyTap(self.special5)
      self.checkStatus()
      
   def test11(self):
      
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
      self.keyTap(self.special6)
      self.keyTap(self.special6)
      self.checkStatus()

   def test12(self):
      
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
      self.keyTap(self.ng_Key_E)
      self.keyTap(self.ng_Key_E)
      self.keyTap(self.ng_Key_E)
      self.checkStatus()
      
   def test13(self):
      
      self.header("|NG_Key_O|*3 : umlaut_O")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyO()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.ng_Key_O)
      self.keyTap(self.ng_Key_O)
      self.keyTap(self.ng_Key_O)
      self.checkStatus()
      
   def test14(self):
      
      self.header("|NG_Key_I|*3 : umlaut_U")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyU()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_I)
      self.checkStatus()
      
   def test15(self):
      
      self.header("|NG_Key_S|*3 : umlaut_S")
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportModifiersActive([keyRAlt()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.ng_Key_S)
      self.keyTap(self.ng_Key_S)
      self.keyTap(self.ng_Key_S)
      self.checkStatus()
      
   def test16(self):
      
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
      self.keyDown(*self.rightThumb2)
      self.scanCycle()
      self.keyDown(*self.ng_Key_A)
      self.scanCycle()
      self.keyUp(*self.ng_Key_A)
      self.keyUp(*self.rightThumb2)
      self.scanCycle()
      self.checkStatus()
      
   def test17(self):
      
      self.header("M1 and a - 2")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyBackslash()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyDown(*self.rightThumb2)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*self.rightThumb2)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyDown(*self.ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*self.ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.checkStatus()

   def test18(self):
      
      self.header("Capitals with one shot shift")
      
      # As the a key arrives in another cycle than the one shot key's
      # activation. First a shift-alone report is send.
      #
      self.queueGroupedReportAssertions([
         ReportAllKeysInactive(),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      # No idea why this report with just the key and no shift
      # is generated. But it seems to work nontheless.
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyDown(*self.leftThumb3)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*self.leftThumb3)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyDown(*self.ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.keyUp(*self.ng_Key_A)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.checkStatus()
      
   def test19(self):

      self.header("i, c")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyI()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyC()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_C)
      
   def test20(self):

      self.header("i, s, c")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyI()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyC()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_S)
      self.keyTap(self.ng_Key_C)
      
   def test21(self):
      
      self.header("Held double s")
      
      # A held double-s should cause the tap timeout to expire 
      # and result in a sequence of s being send
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      # Key s will be kept held
      self.keyTap(self.ng_Key_S)
      self.keyDown(*self.ng_Key_S)
      
      self.skipTime(500)
      self.keyUp(*self.ng_Key_S)
      
   def test22(self):
      
      self.header("Capital umlaut A")
      
      # A held double-s should cause the tap timeout to expire 
      # and result in a sequence of s being send
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportModifiersActive([keyRAlt(), keyLShift()], exclusively = True)
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      # Key s will be kept held
      self.keyTap(self.leftThumb3)
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_I)
      self.keyTap(self.ng_Key_I)
      
   def runTestSeries(self):
      
      #self.test1()
      #self.test2()
      #self.test3()
      #self.test4()
      #self.test5()
      #self.test6()
      #self.test7()
      #self.test8()
      #self.test9()
      #self.test10()
      #self.test11()
      #self.test12()
      #self.test13()
      #self.test14()
      #self.test15()
      #self.test16()
      #self.test17()
      #self.test18()
      #self.test19()
      #self.test20()
      #self.test21()
      self.test22()
      
def main():
    
   test = NoseglassesTest()
   test.debug = True
      
   test.run()
   
   return test
                   
if __name__ == "__main__":
   global test
   test = main()
