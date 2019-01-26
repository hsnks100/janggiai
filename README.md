# janggiai


```cpp
JanggiNode m_node2;
    JanggiAlphaBeta jab2;
    m_node2.string2Layout(stdString);

    jab2.m_pivotPlayer = 2;
    jab2.m_possibleChoice.clear();
    jab2.alphabetaMax(m_node2, JanggiAlphaBeta::m_maxDepth, numeric_limits<int>::min(), numeric_limits<int>::max());
    printf("-oo, +oo ; cutting count : %d\n", jab2.m_cuttingCount);
    actionType best = 0;
    if (jab2.m_possibleChoice.empty() == false)
    {
        int maxscore = -1e9;
        for (auto t : jab2.m_possibleChoice)
        {
            printf("[%d] : %d, %d => %d, %d\n", t.second, getFromY(t.first), getFromX(t.first), getToY(t.first), getToX(t.first));
            if (maxscore < t.second)
            {
                maxscore = t.second;
                best = t.first;
            }
        }
        m_node2.DoMove(best);
    }
    else
    {
        return env->NewStringUTF(""); // 할 초 이 스 가 없 을 때

    }
```
