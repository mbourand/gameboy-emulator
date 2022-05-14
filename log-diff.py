import sys
from venv import create


class GbState:

	def __init__(self):
		self.registers = {}
		self.sp = 0
		self.pc = 0

	def __init__(self, registers, sp, pc):
		self.registers = registers
		self.sp = sp
		self.pc = pc

	def fromRealLine(self, line : str):
		self.registers = {}
		self.pc = int(line[0:4], 16)
		self.sp = int(line[line.find('SP:') + 3:line.find('SP:') + 3 + 4], 16)
		regLetters = ['A', 'F', 'B', 'C', 'D', 'E', 'H', 'L']
		for regLetter in regLetters:
			start = line[37:].find(regLetter + ':') + len(regLetter + ':') + 37
			self.registers[regLetter] = int(line[start:start + 2], 16)

	def fromMeLine(self, line : str):
		self.registers = {}
		self.pc = int(line[0:4], 16)
		self.sp = int(line[line.find('SP:') + 3:line.find('SP:') + 3 + 4], 16)
		regLetters = ['A', 'F', 'B', 'C', 'D', 'E', 'H', 'L']
		for regLetter in regLetters:
			start = line.find(regLetter + ':') + len(regLetter + ':')
			self.registers[regLetter] = int(line[start:start + 2], 16)

	def __eq__(self, other: object) -> bool:
		return self.registers == other.registers and self.sp == other.sp and self.pc == other.pc

	def __str__(self):
		return "PC:" + hex(self.pc) + " SP:" + hex(self.sp) + " B:" + hex(self.registers['B']) + " C:" + hex(self.registers['C']) + " D:" + hex(self.registers['D']) + " E:" + hex(self.registers['E']) + " H:" + hex(self.registers['H']) + " L:" + hex(self.registers['L']) + " F:" + hex(self.registers['F']) + " A:" + hex(self.registers['A'])

class GbStateTest:
	def __init__(self, start, end):
		self.start = start
		self.end = end

	def __eq__(self, other: object) -> bool:
		return self.start == other.start and self.end == other.end

	def __str__(self):
		return "Start: " + str(self.start) + "\nEnd: " + str(self.end)


if len(sys.argv) != 3:
	print("Usage: log-diff.py <my-log> <real-log>")
	sys.exit(1)

seenStates = []
createTest = False

start = 1130000
end = 1160000

with open(sys.argv[2]) as real:
	for i, line in enumerate(real):
		line = line.strip()
		if i > start and i < end:
			if i % 1000 == 0:
				print(i)
			if line.startswith('LR35902'):
				continue
			state = GbState(None, None, None)
			state.fromRealLine(line)

			if createTest:
				seenStates[-1].end = state
				createTest = False

			bannedPcs = [ ]
			if state.pc in bannedPcs:
				continue

			if not "pop " in line and not "ldh [" in line and not "ldh a, [" in line and not state in [seenState.start for seenState in seenStates]:
				seenStates.append(GbStateTest(state, None))
				createTest = True
		if i >= end:
			break
if seenStates[-1].end is None:
	seenStates.pop()
		

checkResult = False
checkResultTest = GbStateTest(None, None)

starts = [seenState.start for seenState in seenStates]
print(f"State amount: {len(starts)}")
with open(sys.argv[1]) as me:
	for i, line in enumerate(me):
		if i < start - 10000:
			continue
		if i % 1000 == 0:
			print(i)
		line = line.strip()
		state = GbState(None, None, None)
		state.fromMeLine(line)

		if checkResult:
			checkResultTest.end = state
			checkResult = False
			if (not checkResultTest in seenStates):
				print(f"Error found line {i}:\nMe:\n" + str(checkResultTest) + "\nReal:\n" + str([seenState for seenState in seenStates if seenState.start == checkResultTest.start][0]) + "\n")
				sys.exit(1)
			else:
				starts.remove(checkResultTest.start)
				seenStates.remove(checkResultTest)

		if state in starts:
			checkResult = True
			checkResultTest = GbStateTest(state, None)
