namespace ZellerDateParser
{
    // (1, 1, 1) => 0
    int zellerId(int y, int m, int d)
    {
        if (m < 3) -- y, m += 12;
        return 365 * y + y / 4 - y / 100 + y / 400
               + (153 * (m - 3) + 2) / 5 + d - 307;
    }

    void zellerDate(int id, int &y, int &m, int &d)
    {
        int x = id + 178995, n, i, j;
        n = 4 * x / 146097, x -= (146097 * n + 3) / 4;
        i = (4000 * (x + 1)) / 1461001, x -= 1461 * i / 4 - 31;
        j = 80 * x / 2447, d = x - 2447 * j / 80, x = j / 11;
        m = j + 2 - 12 * x, y = 100 * (n - 49) + i + x;
    }

    // Sun = 0, Mon ...
    int getWeekday(int id)
    {return (id + 1) & 7;}
}