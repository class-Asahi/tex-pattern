namespace SimpleSplayTree
{
    using T = int;

    struct node
    {
        T key, est, sum;
        int anc, chd[2], cnt;
        int size;
        bool rev;

        void init(T K, int father = 0)
        {
            size = cnt = 1, chd[0] = chd[1] = 0;
            key = est = sum = K, anc = father;
            rev = false;
        }
    };

    class tree
    {
        int root, cnt, mem;
        stack<int> pool;

        using node_t = ::SimpleSplayTree::node;
        using method_t = function<void(node_t&)>;
        vector<node_t> node;

        int &lc(int id) {return node[id].chd[0];}
        int &rc(int id) {return node[id].chd[1];}
        int dir(int id) {return node[node[id].anc].chd[1] == id;}

        int alloc(T key, int father = 0)
        {
            int id = pool.empty() ? ++ cnt : pool.top();
            if (!pool.empty()) pool.pop();
            node[id].init(key, father);
            return id;
        }

        void free(int id) {if (id) pool.push(id);}

        void update(int id)
        {
            auto ls = lc(id), rs = rc(id);
            node[id].size = node[ls].size + node[rs].size + node[id].cnt;
            node[id].est = node[id].key;
            if (ls) node[id].est = min(node[id].est, node[ls].est);
            if (rs) node[id].est = min(node[id].est, node[rs].est);
            node[id].sum = node[id].key * node[id].cnt;
            if (ls) node[id].sum += node[ls].sum;
            if (rs) node[id].sum += node[rs].sum;                 
        }

        void rotate(int id)
        {
            int fa = node[id].anc, gfa = node[fa].anc, d = dir(id);
            if (gfa) node[gfa].chd[dir(fa)] = id;
            node[fa].anc = id, node[id].anc = gfa;
            node[node[id].chd[!d]].anc = fa;
            node[fa].chd[d] = node[id].chd[!d];
            node[id].chd[!d] = fa;
            update(fa), update(id);
        }

        void splay(int id, int goal = 0)
        {
            for (int fa; (fa = node[id].anc) != goal; rotate(id))
                if (node[fa].anc != goal) rotate(dir(id) == dir(fa) ? fa : id);
            if (!goal) root = id;
        }

#ifdef SPLAY_BST_MODE
        int find(T key)
        {
            int id = root;
            if (!id) return 0;
            while (key != node[id].key && node[id].chd[key > node[id].key])
                id = node[id].chd[key > node[id].key];
            return splay(id), id;
        }

        int getNext(T key, bool dir)
        {
            int id = find(key);
            if (dir ? node[id].key > key : node[id].key < key)
                return id;
            id = node[id].chd[dir];
            while (node[id].chd[!dir]) id = node[id].chd[!dir];
            return id;
        }
#endif

        int lazyTag(int id)
        {
#ifdef SPLAY_APPEND_MODE
            if (id && node[id].rev)
            {
                swap(lc(id), rc(id));
                node[id].rev = false;
                auto ls = lc(id), rs = rc(id);
                if (ls) node[ls].rev = !node[ls].rev;
                if (rs) node[rs].rev = !node[rs].rev;
            }
#endif
            return id;
        }

        int atRank(int rank)
        {
            if (rank <= 0 || rank > node[root].size) return -1;
            int id = root, ls = lc(id), rs = rc(id);
            while (lazyTag(id), ls = lc(id), rs = rc(id), id)
                if (rank <= node[ls].size) id = ls;
                else if (rank <= node[ls].size + node[id].cnt) break;
                else rank -= node[ls].size + node[id].cnt, id = rs;
            return id;
        }

        void toStream(ostream &os, int id)
        {
            int ls = lc(id), rs = rc(id);
            if (ls) os << "{", toStream(os, ls), os << "}";
            os << " (#" <<id << ": " << node[id].key << ") ";
            if (rs) os << "{", toStream(os, rs), os << "}";
        }

        void traversal(int id, const method_t &process)
        {
            lazyTag(id);
            const auto ls = lc(id), rs = rc(id);
            if (ls) traversal(ls, process);
            process(node[id]);
            if (rs) traversal(rs, process);
        }

    public:
        explicit tree(int size) : mem(size)
        {
            root = cnt = 0;
            node.resize(size + 1);
        }

        T access(int id) {return node[id].key;}

        void clear()
        {
            root = cnt = 0;
            while (!pool.empty()) pool.pop();
        }

        T at(int rank)
        {
            if (node[root].size < rank) return -1;
            auto id = atRank(rank);
            return splay(id), node[id].key;
        }

        int getSize() {return root ? node[root].size : 0;}

        void toArray(vector<T> &arr)
        {
            arr.clear();
            traversal(root, [&](node_t &ii){arr.push_back(ii.key);});
        }

        /**
         * BST 模式：
         * 
         * 树始终维持了 BST 的性质，可以使用二分法在树上查找；
         * erase 方法已经换成不要求哨兵节点的版本了；
         * 对于每一个节点，使用 cnt 域维护该值的出现次数；
         */
#ifdef SPLAY_BST_MODE
        void insert(T key)
        {
            if (!root)
            {
                root = alloc(key);
                return;
            }
            int id = root, fa = 0;
            while (id && node[id].key != key)
            {
                fa = id;
                id = node[id].chd[key > node[id].key];
            }
            if (id) return ++ node[id].cnt, splay(id);
            id = alloc(key, fa);
            if (fa) node[fa].chd[key > node[fa].key] = id;  // 无需特判 fa 是否为根
            return splay(id);
        }

        void erase(T key)
        {
            int id = find(key), ex;
            if (!id || node[id].key != key) return;
            if (node[id].cnt > 1)
                return -- node[id].cnt, update(id);
            auto ls = lc(id), rs = rc(id);
            if (!ls || !rs)
            {
                root = ls | rs;
                if (root) node[root].anc = 0;
                return free(id);
            }
            id = ls, ex = root, rs = rc(id);
            while (rs) id = rs, rs = rc(id);
            splay(id), node[id].anc = 0, rs = rc(ex);
            node[rs].anc = id, rc(id) = rs;
            return free(ex), update(root);
        }

        int getOrder(T key)
        {
            int id = find(key);
            if (!lc(id)) return 1;
            return node[lc(id)].size + 1;
        }
        
        T getPrecursor(T key)
        {
            auto id = getNext(key, 0);
            return node[id].key;
        }

        T getSucceed(T key)
        {
            auto id = getNext(key, 1);
            return node[id].key;
        }
#endif

        /**
         * APPEND 模式：
         *
         * 指不适用 BST 性质，仅将树的中序遍历作为数组序列的用法；
         * 因此，不能使用 find 方法查找特定键值的节点 id；
         * 可通过使用第 k 大的方法去按照下标来获得 id 从而随机访问；
         * 还可以通过维护关于键值的一些特定值，来进行关于键值的特定查找；
         *
         * 当在 APPEND 模式下，节点的 cnt 值毫无意义，始终为 1；
         * 并不是不可以使用 cnt 维护连续的值相同的区间，主要是意义不大；
         */
#ifdef SPLAY_APPEND_MODE
        void append(T key)
        {
            if (!root)
            {
                root = alloc(key);
                return;
            }
            int id = root, rs;
            while (rs = rc(id), id)
                if (!rs) return splay(rc(id) = alloc(key, id));
                else id = rs;
        }

        void remove(int id)
        {
            splay(id);                      // assert(id != 0)
            int ls = lc(id), rs = rc(id);   // 不要滥用引用！ ls/rs 在 id 后更新
            if (node[id].cnt > 1)
                return -- node[id].cnt, update(id);
            if (node[id].size == 1)
                return root = 0, free(id);
            if (!ls || !rs)
                return root = ls | rs, node[root].anc = 0, free(id);
            id = node[root].chd[0], ls = lc(id), rs = rc(id);
            while (rs) id = rs, ls = lc(id), rs = rc(id);
            splay(id), free(rs);            // free 没有立即清空，所以可以先 free
            ls = lc(id), rs = rc(id);       // 树的结构发生变化，应立即更新 ls， rs
            node[rc(rs)].anc = id, rc(id) = node[rs].chd[1];
            update(id);
        }

        int getLast(int id)
        {
            splay(id), id = lc(id);
            while (rc(id)) id = rc(id);
            return id;
        }

        int getNext(int id)
        {
            splay(id), id = rc(id);
            while (lc(id)) id = lc(id);
            return id;
        }

        // 使用的时候应当保证插入虚节点作为哨兵（仅末尾
        void reverse(int l, int r)
        {
            if (l > r) return;
            int id = atRank(r + 1);
            splay(id), splay(atRank(l - 1), id);
            int son = rc(lc(root));
            node[son].rev = !node[son].rev;
        }
#endif

        T operator [](int rank)
        {
            int id = atRank(rank);
            splay(id);
            return node[id].key;
        }

        friend ostream &operator <<(ostream &os, tree &me)
        {
            me.toStream(os, me.root);
            return os << flush;
        }
    };

}