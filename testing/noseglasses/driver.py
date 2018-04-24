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
   
   def keyDownWait(self, key_name):
      
      self.log("Pressing key %s" % key_name)
      
      key = getattr(self, key_name)
            
      self.keyDown(*key)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      
   def keyUpWait(self, key_name):
            
      self.log("Releasing key %s" % key_name)
      
      key = getattr(self, key_name)
      
      self.keyUp(*key)
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()

   def keyTap(self, key):
            
      self.keyDownWait(key)
      self.keyUpWait(key)
      
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
      
      self.special1 = (2,  9)
      self.special2 = (0,  9)
      self.special3 = (0,  6)
      self.special4 = (1,  6)
      self.special5 = (2,  6)
      self.special6 = (1,  9)
      
      self.ng_Key_E = (2,  3)
      self.ng_Key_O = (2, 13)
      self.ng_Key_I = (2, 12)
      self.ng_Key_S = (2,  2)
      
      self.ng_Key_A = (2,  1)
      self.ng_Key_C = (3,  3)
      self.ng_Key_X = (3,  2)
      self.ng_Key_Z = (3,  1)
            
      self.ng_Key_Quote = (3, 13)
            
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
      self.keyTap("leftThumb3")
      self.keyTap("rightThumb2")
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
      self.keyTap("leftThumb3")
      self.keyTap("leftThumb3")
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
      self.keyTap("leftThumb3")
      self.keyTap("rightThumb3")
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
      self.keyTap("rightThumb2")
      self.keyTap("rightThumb2")
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
      self.keyTap("rightThumb2")
      self.keyTap("leftThumb2")
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
      self.keyTap("special1")
      self.keyTap("special1")
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
      self.keyTap("special3")
      self.keyTap("special3")
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
      self.keyTap("special3")
      self.keyTap("special3")
      self.keyTap("special3")
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
      self.keyTap("special4")
      self.keyTap("special4")
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
      self.keyTap("special5")
      self.keyTap("special5")
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
      self.keyTap("special6")
      self.keyTap("special6")
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
      self.keyTap("ng_Key_E")
      self.keyTap("ng_Key_E")
      self.keyTap("ng_Key_E")
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
      self.keyTap("ng_Key_O")
      self.keyTap("ng_Key_O")
      self.keyTap("ng_Key_O")
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
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_I")
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
      self.keyTap("ng_Key_S")
      self.keyTap("ng_Key_S")
      self.keyTap("ng_Key_S")
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
      self.keyDownWait("rightThumb2")
      self.keyDownWait("ng_Key_A")
      self.keyUpWait("ng_Key_A")
      self.keyUpWait("rightThumb2")
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
      self.keyDownWait("rightThumb2")
      self.keyUpWait("rightThumb2")
      self.keyDownWait("ng_Key_A")
      self.keyUpWait("ng_Key_A")
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
      self.keyDownWait("leftThumb3")
      self.keyUpWait("leftThumb3")
      self.keyDownWait("ng_Key_A")
      self.keyUpWait("ng_Key_A")
      self.checkStatus()
      
   def test19(self):

      self.header("i, c")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyI()], exclusively = True),
         ReportAllModifiersInactive()
      ])     
      #self.queueGroupedReportAssertions([
         #ReportKeysActive([keyI(), keyC()], exclusively = True),
         #ReportAllModifiersInactive()
      #])
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
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_C")
      
      self.checkStatus()
      
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
      #self.queueGroupedReportAssertions([
         #ReportKeysActive([keyS(), keyC()], exclusively = True),
         #ReportAllModifiersInactive()
      #])
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
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_S")
      self.keyTap("ng_Key_C")
      
      self.checkStatus()
      
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
      self.keyTap("ng_Key_S")
      self.keyDown(*self.ng_Key_S)
      
      self.skipTime(500)
      self.checkStatus()
      
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyUp(*self.ng_Key_S)
      
      self.scanCycle()
      self.scanCycle()
      self.scanCycle()
      self.checkStatus()
      
   def test22(self):
      
      self.header("Capital umlaut A")
      
      # A held double-s should cause the tap timeout to expire 
      # and result in a sequence of s being send
      
      self.queueGroupedReportAssertions([
         ReportAllKeysInactive(),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
            
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyU()], exclusively = True),
         ReportModifiersActive([keyRAlt(), keyLShift()], exclusively = True)
      ])
      
      # The OneShot shift is removed but the rest remains until the
      # i-key is released.
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyU()], exclusively = True),
         ReportModifiersActive([keyRAlt(), ], exclusively = True)
      ])

      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      # Make sure that finally, everything continues to work normally
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyI()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      self.keyTap("leftThumb3")
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_I")
      self.keyTap("ng_Key_I")
      
      self.keyTap("ng_Key_I")
      
      self.skipTime(500)
      self.checkStatus()
      
   def test23(self):
      
      self.header("OneShot C")
           
      self.queueGroupedReportAssertions([
         ReportAllKeysInactive(),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyC()], exclusively = True),
         ReportModifiersActive([keyLShift()], exclusively = True)
      ])
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyC()], exclusively = True),
         ReportAllModifiersInactive()
      ])
         
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      self.keyDownWait("leftThumb3")
      self.keyDownWait("ng_Key_C")
      self.keyUpWait("leftThumb3")
      self.keyUpWait("ng_Key_C")
      self.checkStatus()
      
   def test24(self):
      
      self.header("x")
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyX()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      self.keyTap("ng_Key_X");
      
      self.checkStatus()
   
   def test25(self):
      
      self.header("z")
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyZ()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      self.keyTap("ng_Key_Z");
      
      self.checkStatus()
      
   def test26(self):
      
      self.header("s, e")
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyS(), keyE()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      
      #self.queueGroupedReportAssertions([
         #ReportEmpty()
      #])
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyE()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      
      self.keyDownWait("ng_Key_S")
      #self.skipTime(500)
      self.keyDownWait("ng_Key_E")
      self.keyUpWait("ng_Key_S")
      self.skipTime(500)
      self.keyUpWait("ng_Key_E")
      self.skipTime(500)
      self.checkStatus()
         
   def test27(self):

      self.header("e, a")
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyE()], exclusively = True),
         ReportAllModifiersInactive()
      ])     
      #self.queueGroupedReportAssertions([
         #ReportKeysActive([keyI(), keyC()], exclusively = True),
         #ReportAllModifiersInactive()
      #])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap("ng_Key_E")
      self.keyTap("ng_Key_A")
      
      self.checkStatus()

   def test28(self):

      self.header("e, a - 2")
      
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyE()], exclusively = True),
         ReportAllModifiersInactive()
      ])     
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyE(), keyA()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      #self.queueGroupedReportAssertions([
         #ReportEmpty()
      #])
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyA()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])   
            
      self.keyDownWait("ng_Key_E")
      #self.skipTime(500)
      self.keyDownWait("ng_Key_A")
      self.keyUpWait("ng_Key_E")
      self.skipTime(500)
      self.keyUpWait("ng_Key_A")
      self.skipTime(500)
      self.checkStatus()
      
   def test29(self):
      
      self.header("Double s with timeout")
      
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
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyTap("ng_Key_S")
      self.keyTap("ng_Key_S")
      
      self.skipTime(500)
      self.checkStatus()
      
   def test30(self):
      
      self.header("'sx")
      
      # A held double-s should cause the tap timeout to expire 
      # and result in a sequence of s being send
      #
      self.queueGroupedReportAssertions([
         ReportKeysActive([keyQuote()], exclusively = True),
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
         ReportKeysActive([keyX()], exclusively = True),
         ReportAllModifiersInactive()
      ])
      self.queueGroupedReportAssertions([
         ReportEmpty()
      ])
      self.keyDownWait("rightThumb2")
      self.keyDownWait("ng_Key_Quote")
      self.keyUpWait("rightThumb2")
      self.keyDownWait("ng_Key_S")
      self.keyUpWait("ng_Key_S")
      self.keyUpWait("ng_Key_Quote")
      self.keyTap("ng_Key_X")
      self.checkStatus()
      
   def runTestSeries(self):
      
      self.test1()
      self.test2()
      self.test3()
      self.test4()
      self.test5()
      self.test6()
      self.test7()
      self.test8()
      self.test9()
      self.test10()
      self.test11()
      self.test12()
      self.test13()
      self.test14()
      self.test15()
      self.test16()
      self.test17()
      self.test18()
      self.test19()
      self.test20()
      self.test21()
      self.test22()
      self.test23()
      self.test24()
      self.test25()
      self.test26()
      self.test27()
      self.test28()
      self.test29()
      self.test30()
      
def main():
    
   test = NoseglassesTest()
   test.debug = True
      
   test.run()
   
   return test
                   
if __name__ == "__main__":
   global test
   test = main()
