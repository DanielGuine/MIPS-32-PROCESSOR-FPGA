0-jump 27
Função somatorio:
1-sw r29 r31 1
2-addi r30 r30 1
3-addi r30 r30 1
4-addi r30 r30 1
5-addi r30 r30 1
6-li r26 0
7-sw r29 r26 3
8-li r26 1
9-sw r29 r26 4
L0:
10-lw r29 r1 4
11-lw r29 r2 2
12-slte r1 r2 r3
13-beq r3 r0 22
14-lw r29 r4 3
15-lw r29 r5 4
16-ADD r4 r5 r6
17-sw r29 r6 3
18-lw r29 r7 4
19-ADDi r7 r8 1
20-sw r29 r8 4
21-jump 10
L1:
22-lw r29 r9 3
23-move r9 r28 0
24-jump 25
25-lw r29 r31 1
26-jr r31 r0 r0
Função main:
27-addi r30 r30 1
28-addi r30 r30 1
29-input r28
30-move r28 r1 0
31-sw r29 r1 0
32-lw r29 r2 0
33-sw r30 r29 0
34-move r30 r29 0
35-addi r30 r30 1
36-sw r29 r2 2
37-jal 1
38-move r29 r30 0
39-lw r29 r29 0
40-subi r30 r30 1
41-move r28 r3 0
42-sw r29 r3 1
43-lw r29 r4 1
44-output r4
45-halt
