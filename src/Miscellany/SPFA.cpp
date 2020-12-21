namespace SPFA
{
    using namespace FWS;

    require int n;

    template <class _n_Array>
    bool bfsVersion(int st, _n_Array &dis)
    {
        for (int i = 1; i <= n; ++ i) dis[i] = inf;
        bitset<N> inQue;
        queue<int> hot;
        vector<int> path(n + 1);
        dis[st] = 0, inQue[st] = true, 
        path[st] = 1, hot.push(st);
        bool nega = false;
        while (!hot.empty())
        {
            int u = hot.front();
            hot.pop(), inQue[u] = false;            //// 出队后取消标记
            forEach(u, [&](const edge &e)
            {
                if (nega) return;
                auto [uu, v, w, x] = e;
                if (dis[u] + w >= dis[v]) return;   //// TIP: 三角不等式松弛
                else dis[v] = dis[u] + w;
                path[v] = path[u] + 1;              //// 维护路径上最大可能的节点个数
                if (path[v] > n) return void(nega = true);  //// 发现负环
                if (inQue[v]) return;
                inQue[v].flip(), hot.push(v);       //// 和 dij 不同，不需要现在的路径长度
            });
            if (nega) break;
        }
        return !nega;           //// 没有发现负环
    }

    /**
     * 
     * 如果存在负环，那么 DFS 版本比 BFS 要快
     * 否则；DFS 版本比 BFS 要慢得多，还请多加注意
     * 
     * TODO: 没有验证！
     */
    template <class _n_Array>
    bool dfsVersion(int st, _n_Array &dis)
    {
        for (int i = 1; i <= n; ++ i) dis[i] = inf;
        stack<int> hot;
        bitset<N> vis;
        dis[st] = 0;
        bool hasNega = false;
        const function subDFS = [&](int u)
        {
            vis[u] = true;
            forEach(u, [&](const edge &e)
            {
                auto [uu, v, w, x] = e;
                if (dis[u] + w >= dis[v]) return;
                if (vis[v]) return void(hasNega = true);
                else dis[v] = dis[u] + w;
                subDFS(v);
            });
            vis[u] = false;
        };
        return subDFS(st), !hasNega;
    }
}