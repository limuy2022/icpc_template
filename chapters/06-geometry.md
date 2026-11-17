# 计算几何 · 二维基础与精度

按 OI Wiki 计算几何目录的 13 个专题编排，覆盖二维与三维基础、距离、Pick、三角剖分、凸包、扫描线、卡壳、半平面交、最近点对、随机增量、最小圆覆盖、反演。

几何页码：二维基础 34、线与多边形 35、圆 36、三维 37、距离与 Pick 38、凸包 39、扫描线 40、卡壳 41、半平面交 42、最近点对 43、最小圆覆盖 44、三角剖分 45、反演 46、有理数 47。

## 点、向量与共用函数

[C++ 源码](../templates/06-geometry/point.cpp)

- cross(B-A,C-A)>0 表示 C 在 A→B 左侧，正向多边形为逆时针。dot 为 0 表示垂直，cross 为 0 表示平行。
- 整数坐标优先用整数叉积。先提升再相减，如 (i128(b.x)-a.x)*(i128(c.y)-a.y)，否则减法可能先溢出。
- EPS 必须结合坐标尺度与误差要求选择。叉积有长度平方量纲，点积与长度的阈值不能盲目混用。
- sort 的比较器必须严格弱序，不能用“差小于 EPS 则相等”的规则排序。排序用精确字典序，几何判定另用 EPS。
- 求角度优先 atan2(cross,dot)。acos 的参数先 clamp 到 [-1,1]。零向量没有方向或夹角。

## 三角形与极角序

- $S=\frac{1}{2}|\operatorname{cross}(B-A,C-A)|$，正弦定理 $\frac{a}{\sin A}=2R$，余弦定理 $\cos A=\frac{b^2+c^2-a^2}{2bc}$。
- 三边 a,b,c，半周长 s，面积 $\sqrt{s(s-a)(s-b)(s-c)}$。内切半径 r=面积/s，外接半径 $R=\frac{abc}{4S}$。
- 极角排序先分上半平面（y>0 或 y=0 且 $x\geq0$），再按叉积正负比较，同向按长度破平局。零向量单独放置。
- 角度跨越$-\pi $与$\pi $的断点，直接比较 atan2 前先确定使用的区间。浮点精度允许时 atan2 排序较易核对。

来源范围按 OI Wiki Team 计算几何目录核对，2026-11-05。以下几何代码共享 Point 与基础函数，使用前按数据范围设置精度。

# 计算几何 · 线段、直线与多边形

## 相交、投影与距离

[C++ 源码](../templates/06-geometry/segment.cpp)

- `line_intersection` 的参数是“点+方向”。平行与重合都返回空，需要时另判 cross(b-a,v)=0。
- 线段距离相交为 0，否则取四个端点到另一线段的最小距离。射线投影要求参数$t\geq 0$。

## 多边形面积、重心、点包含

- 有向双倍面积 $A_2=\sum_i\operatorname{cross}(p_i,p_{i+1})$，面积为 $\frac{|A_2|}{2}$，周长为 $\sum_i|p_{i+1}-p_i|$。
- 非零面积简单多边形的重心 $C=\frac{\sum_i(p_i+p_{i+1})\operatorname{cross}(p_i,p_{i+1})}{3A_2}$。
- 点包含先判边界，再按射线奇偶或回转数判内部。上、下端点采用半开高度区间，避免顶点算两次。

[C++ 源码](../templates/06-geometry/polygon.cpp)

- 凸多边形可用固定顶点扇形二分，将单点判定降为$\mathcal{O}(\log  n)$，须先处理扇形两边界和退化凸包。

# 计算几何 · 圆与切线

## 直线与圆、两圆交点

- 直线投影 H=proj(O,AB)，d=|O-H|。d>r 无交，d=r 相切，d<r 时交点为 $H\pm\frac{B-A}{|B-A|}\sqrt{r^2-d^2}$。
- 两圆中心$\mathcal{O}_{1}$、$\mathcal{O}_{2}$，$d=|\mathcal{O}_{2}-\mathcal{O}_{1}|$。同心时先判是否重合；非同心时，$d>r_{1}+r_{2}$外离，$d<|r_{1}-r_{2}|$内含。
- $a=\frac{r_1^2-r_2^2+d^2}{2d}$，$H=O_1+(O_2-O_1)\frac{a}{d}$，$h^{2}=r_{1}^{2}-a^{2}$，交点 $H\pm\operatorname{perp}(O_2-O_1)\frac{\sqrt{h^2}}{d}$。
- 相切只保留一个交点。因舍入得到很小的负$h^{2}$时取$\max (0,h^{2})$，明显负值须判无交。

## 点到圆切线、两圆公切线

- 点 P 在圆外，设 v=P-O、$d^{2}=|v|^{2}$。切点 $O+\frac{r^2}{d^2}v\pm\frac{r\sqrt{d^2-r^2}}{d^2}\operatorname{perp}(v)$。
- 点在圆上只有一个切点，圆内无实切线。P=O 须先排除。
- 两圆切线可枚举第二圆的有向半径$r_{2}$或$-r_{2}$。令$(\Delta r)=r_{1}-r_{2}$、$v=\mathcal{O}_{2}-\mathcal{O}_{1}$，要求$|v|^{2}\geq (\Delta r)^{2}$。
- 法向量 $\boldsymbol{n}=\frac{v\,\Delta r\pm\operatorname{perp}(v)\sqrt{|v|^2-(\Delta r)^2}}{|v|^2}$，切点为$\mathcal{O}_{1}+n r_{1}$与$\mathcal{O}_{2}+n r_{2}$。有向半径取负给内公切线。

## 圆交面积与角度

- 两圆外离面积为 0，内含面积为较小圆面积，同心单独处理。
- 一般情况下 $\alpha=\arccos\frac{d^2+r_1^2-r_2^2}{2dr_1}$，$\beta $同理，面积 $r_1^2\alpha+r_2^2\beta-dr_1\sin\alpha$。
- 弧长$r\theta $，扇形面积 $\frac{r^2\theta}{2}$，$\theta $用弧度。圆弓形有向面积 $\frac{r^2}{2}(\theta-\sin\theta)$。

## 圆与多边形面积交

- 将圆心平移为原点，逐条有向边求“原点-边”三角形与圆的有向交面积再求和。
- 用线段与圆的交点参数把边分段。子段中点在圆内时贡献 cross(A,B)/2；在圆外时贡献 $\frac{r^2}{2}\operatorname{atan2}(\operatorname{cross}(A,B),\operatorname{dot}(A,B))$。
- 线段退化、切点、端点在圆上与跨越负 x 轴时都要保持有向角。最终面积取绝对值。
- 二次方程求交参数只保留 [0,1] 内并去重，将 0、1 加入排序后逐段积分。

## 常见退化检查

- 半径必须非负。r=0 是点，可独立处理。重复点不能直接归一化方向向量。
- 输入含完全重合圆时，交点不是有限集合，输出结构要区分“无交、有限交、重合”。
- 圆切线、圆交等公式在浮点下容易出现 -0，输出可将绝对值小于输出精度的数置 0。

# 计算几何 · 三维基础与三维凸包

## 向量与直线、平面

- 三维 $\operatorname{dot}(a,b)=\sum_i a_ib_i$。$a\times b=(a_yb_z-a_zb_y,\ a_zb_x-a_xb_z,\ a_xb_y-a_yb_x)$。
- 混合积 [a,b,c]=dot(a,cross(b,c))。四面体有向六倍体积为 [B-A,C-A,D-A]，绝对值除以 6。
- 平面写成 n·X=d，n 非零。三点 A,B,C 给 n=(B-A)×(C-A)，d=n·A；共线时不能定义唯一平面。
- 点到平面距离 $\frac{|n\cdot P-d|}{|n|}$，投影 $P-\frac{n\cdot P-d}{|n|^2}n$。
- 直线 P+tV 与平面交参数 $t=\frac{d-n\cdot P}{n\cdot V}$。分母为 0 时需区分平行无交与直线在面内。
- 两平面交线方向 n1×n2。方向为零则平行或重合，否则选一个坐标固定，解另两个坐标的 2×2 方程。

## 夹角、异面线距离

- 两直线的锐夹角 $\cos\theta=\frac{|u\cdot v|}{|u||v|}$。有向向量夹角不取绝对值。
- 线与面夹角 $\sin\theta=\frac{|v\cdot n|}{|v||n|}$，两平面的锐夹角看两法向量。
- 异面直线 P+tu、Q+sv 的距离为 $\frac{|(Q-P)\cdot(u\times v)|}{|u\times v|}$；平行时改用 $\frac{|(Q-P)\times u|}{|u|}$。
- 线段到线段还要约束两个参数在 [0,1]，无约束最近点落在外部时回到边界端点距离问题。
- 三面角的面角 a,b,c 对应对二面角 A,B,C，有 $\frac{\sin a}{\sin A}=\frac{\sin b}{\sin B}=\frac{\sin c}{\sin C}$。另有 $\cos a=\cos b\cos c+\sin b\sin c\cos A$。

## 增量三维凸包

```text
find four non-coplanar points and build an outward-oriented tetrahedron
for each remaining point P:
    mark each face ABC visible if dot(cross(B-A,C-A), P-A) > 0
    delete all visible faces
    find horizon edges shared by one visible and one invisible face
    connect each horizon edge to P, orienting new triangles outward
```

- 初始点先去重，再找不共线的三点、不共面的第四点。仿射维度为 0/1/2 时分别退化成点、线段、平面凸包。
- 维护有向边邻接，删除可见面内部的边，地平线边才能接新面。所有面的方向必须一致。
- 保留初始四面体内部点可检查新面朝向，该点始终在最终凸包内。随机打乱能改善一般增量算法的表现。
- 简单扫描所有面的增量法最坏$\mathcal{O}(n^{2})$，三维凸包面数为$\mathcal{O}(n)$。不应将随机打乱直接当作最坏$\mathcal{O}(n \log  n)$保证。
- 封闭有向三角面凸包的体积为 $\frac{1}{6}\left|\sum\operatorname{dot}(A,\operatorname{cross}(B,C))\right|$，表面积为 $\frac{1}{2}\sum|\operatorname{cross}(B-A,C-A)|$。

# 计算几何 · 距离与 Pick 定理

## 距离变换

- 欧氏距离 $L_2(P,Q)=\sqrt{\sum_i(p_i-q_i)^2}$。比较远近优先比较平方，避免不必要的开方。
- 曼哈顿 $L_1(P,Q)=\sum_i|p_i-q_i|$，切比雪夫 $L_\infty=\max_i|p_i-q_i|$。一般 Minkowski $L_p(P,Q)=\left(\sum_i|p_i-q_i|^p\right)^{1/p}$，$p\geq 1$。
- 二维令 u=x+y、v=x-y，有$L_{1}(P,Q)=\max (|\Delta u|,|\Delta v|)$。$L_\infty(P,Q)=\frac{|\Delta u|+|\Delta v|}{2}$。
- 逆变换 $x=\frac{u+v}{2}$、$y=\frac{u-v}{2}$，整数格点要求 u、v 同奇偶。数点时不能忽略此限制。
- 二维曼哈顿最远点对距离为 max(max(x+y)-min(x+y),max(x-y)-min(x-y))。
- d 维$L_{1}$最远距离为 $\max_{s\in\{-1,1\}^{d}}\left(\max_x\sum_i s_ix_i-\min_x\sum_i s_ix_i\right)$，可固定一个符号减半枚举。

## Pick 定理与边界格点

- 顶点均为整数的简单无洞多边形，面积 $S=I+\frac{B}{2}-1$。I 为严格内部格点数，B 为边界格点数。
- 边从$(x_{1},y_{1})$到$(x_{2},y_{2})$，两端均计时有$\gcd (|\Delta x|,|\Delta y|)+1$个格点。多边形环边求和时每条只计一个端点，$B=\sum_{\mathrm{edges}}\gcd(|\Delta x|,|\Delta y|)$。
- 用有向双倍面积$A_{2}$，$I=\frac{|A_2|-B+2}{2}$。整数算到底，不需要浮点面积。
- 有 h 个洞，推广为 $S=I+\frac{B}{2}-1+h$，$I=\frac{2S-B+2-2h}{2}$。B 要计所有边界环。

[C++ 源码](../templates/06-geometry/pick.cpp)

- 仅用于非退化、简单、无洞格点多边形。自交多边形的代数面积不能直接代入 Pick。
- 坐标差若可能达到 `LLONG_MIN`，llabs 不可表示，先用 i128 算差并取绝对值，再用 std::__gcd。
- 坐标很大时，双倍面积累加也可能超过 i128，按 n 与坐标界估算。

# 计算几何 · 二维凸包与 Minkowski 和

## Andrew 单调链（只留极点，逆时针）

[C++ 源码](../templates/06-geometry/convex-hull.cpp)

- 时间$\mathcal{O}(n \log  n)$。全共线返回两端点；重复单点返回一个点；空输入返回空。
- 要保留边上的共线点，将$\leq 0$改成 <0，同时单独处理全共线，避免上下链重复输出。
- Graham 先选最低点再极角排序后栈扫描，同样$\mathcal{O}(n \log  n)$，共线排序和尾段顺序更易出错。
- 凸多边形周长含退化线段时常为 2·线段长，题目也可能要求线段长，须按定义处理。

## Minkowski 和

- A+B={a+b}，凸集的和仍凸。两凸多边形均逆时针，边向量按极角归并，可$\mathcal{O}(n+m)$构造和的边界。
- 先去连续共线冗余点，将各自最低 y、再最低 x 的顶点旋转到首位。初始点为两个首点之和。
- 比较下一条边方向，较小者推进；同向则两条边相加并同时推进。走完所有边后删除重复尾点。
- 点或线段输入单独处理。反射 -B 后的 A+(-B) 可把两凸多边形相交化为“原点在差集内”。
- 凸多边形间最短距离也等于原点到 A+(-B) 的距离，若原点在差集内则为 0。
- 用边归并时必须有完整极角序（半平面加叉积），单独看叉积不能全局比较相反方向。

# 计算几何 · 扫描线与矩形面积并

## 离散化 y、维护实际覆盖长度

矩形为整数坐标$(x_{1},y_{1},x_{2},y_{2})$，默认$x_{1}\leq x_{2}$、$y_{1}\leq y_{2}$。退化矩形跳过。线段树叶子代表 [Y[i],Y[i+1])，不是一个坐标点。

[C++ 源码](../templates/06-geometry/rectangle-union.cpp)

- $\mathcal{O}(n \log  n)$时间、$\mathcal{O}(n)$空间。面积贡献先用旧覆盖长度乘横向位移，再处理本 x 的全部事件。
- 周长并还需维护覆盖段数与端点覆盖标记。竖向贡献要处理同 x 的增删重合，不能只用组前后总长度差。
- 二维离线数点按 x 排序，将 y 压缩，用区间计数结构维护。矩形查询拆为四个前缀，边界统一$\leq $或 <。

# 计算几何 · 旋转卡壳

## 凸包直径（平方距离）

输入为逆时针凸包且仅保留极点，重复首点不放在末尾。空或单点返回 0，两点直接计算。

[C++ 源码](../templates/06-geometry/rotating-calipers.cpp)

- 凸包生成$\mathcal{O}(n \log  n)$，卡壳本身$\mathcal{O}(h)$。相等的最远支撑点可能有两个，因此检查 j 与 j+1。
- 若原数据是整数，面积比较与距离平方可用 i128，得到精确最远点对。

## 最小面积外接矩形

- 最优矩形至少有一条边与凸包某条边平行。枚举边方向$u=\mathrm{unit}(p_{i+1}-p_i)$，v=perp(u)。
- 令 L=min dot(p,u)、R=max dot(p,u)、B=min dot(p,v)、T=max dot(p,v)，矩形面积 (R-L)(T-B)。
- 四角分别为 uL+vB、uR+vB、uR+vT、uL+vT。此处点已按同一全局坐标投影。
- 对逆时针凸包，方向旋转时四个极值点单调前进，可用四指针把每次$\mathcal{O}(h)$扫描降到总$\mathcal{O}(h)$。
- 指针推进按下一点的投影严格改善，等号时统一选择规则并限制每轮最多 h 次，避免共线导致死循环。
- 点的最小面积为 0，线段的宽为 0，外接矩形退化。周长最小矩形可用同类支撑，但比较目标改为 2(宽+高)。

## 平行支撑与凸集距离

- 一条边及其最远对点构成平行支撑，得到该方向的宽。最小宽度枚举凸包边即可。
- 两凸多边形距离可用卡壳，也可转成 Minkowski 差后求原点到边界的最短距离。
- 卡壳依赖凸性与有序边界。先对任意点集求凸包，再应用上述单调性。

# 计算几何 · 半平面交

## 有向线左侧约定与有界裁剪

保留$\mathrm{cross}(B-A,P-A)\geq 0$的一侧。下面对已有凸多边形逐条裁剪，适合约束数中等或已知初始边界的题。

[C++ 源码](../templates/06-geometry/half-plane-clip.cpp)

- 此模板用原始符号判断边界，确保交点参数在 [0,1]。浮点噪声较大时，按题意将边界统一平移一个容差后再裁剪。
- 从题目给定的固定顶点数外框开始，每条半平面裁剪一次，最坏$\mathcal{O}(k^{2})$。初始有 h 个顶点时为$\mathcal{O}(k(h+k))$，结果可能为空、点或线段。
- 用人工大矩形框住无界解，会改变问题。只有已知所有合法答案都在框内，才可把框当作真正约束。

## 排序加双端队列，$\mathcal{O}(k \log  k)$

```text
sort directed lines by direction angle
merge same-direction parallel lines, retaining the stricter left half-plane
for each line L:
    while intersection(last two lines) is outside L: pop back
    while intersection(first two lines) is outside L: pop front
    push L
finish by pruning both ends against the opposite end line
intersect each pair of adjacent deque lines, including last with first
```

- 判断同方向先看叉积为 0，再看点积>0。反向平行不属于同一组，可能形成条带，也可能互相矛盾。
- 相同方向时，若 cross(v,b-a)>0，经过 b 的左半平面比经过 a 的更严格。
- 浮点角度可用 atan2 排序，平行处理单独做容差判断。不要把 EPS 相等放进 sort 比较器。
- 队列中相邻平行线不能直接求交。需把空集、无界条带、低维可行集与有面积多边形分开表达。

## 多边形的核

- 简单多边形逆时针时，对每条边取左半平面，相交得到能看到整个多边形的点集。顺时针须先反向。
- 核为空则不是星形多边形；非空核可能退化。多边形自身的包围盒是合法外框，可用于有界裁剪。
- 严格半平面与闭半平面在切边、点接触时答案不同，按题目要求统一不等号。

# 计算几何 · 平面最近点对

## 分治与归并，$\mathcal{O}(n \log  n)$

[C++ 源码](../templates/06-geometry/closest-pair.cpp)

- 递归入口按 x 划分，出口将该段排成 y 序。必须先保存 `middle_x`，不能递归后再读被重排的位置。
- 带状区按 y 排序，平面装箱界保证每点只检查常数个有效邻点。重复点的最近距离为 0。
- 返回平方距离，最后输出时再 sqrt。整数输入若要完全精确，可改距离和比较为宽整数。

## 其他路线与推广

- 扫描线按 x 排序，用按 y 的有序集合保留 x 距离小于当前最佳值的点，在 y 的窄区间中查询。
- 随机网格算法随机打乱，以当前距离为网格尺度；最佳距离改变时重建网格，期望线性但对哈希和退化输入更敏感。
- 最小周长三角形也可用分治带状筛选，但阈值与常数邻域需重新证明，不能把点对代码直接换成三重循环。

# 计算几何 · 随机增量与最小圆覆盖

## 三点外接圆与三层循环

[C++ 源码](../templates/06-geometry/minimum-enclosing-circle.cpp)

- 空集返回半径 -1 作为无圆标记，单点半径为 0。共线三点用最远两点的直径圆。
- den 接近 0 时外接圆数值敏感，必要时用更高精度或按数据误差做共线分类。不能随意把所有瘦三角形当作共线。
- 随机打乱后期望$\mathcal{O}(n)$，确定输入顺序下三层循环最坏$\mathcal{O}(n^{3})$。固定随机种子便于复现测试。

## 为什么随机增量能加速

- 最优圆由最多 3 个边界点确定，随机前缀中的新点成为边界的概率小，内层重建的高成本被低触发概率摊薄。
- 先判是否违反现有解，违反才固定该点为新的边界约束；两层、三层分别对应固定两个、三个边界点。
- Delaunay、增量凸包也使用随机顺序与冲突结构。每个问题都需单独证明局部更新正确和期望工作量。

# 计算几何 · 三角剖分与 Voronoi

## 简单多边形与点集剖分

- 无洞简单 n 边形可分成 n-2 个三角形，用 n-3 条互不相交的内部对角线。带 h 个洞时需区分桥接顶点的重复计数。
- 耳切法删除一个凸耳，三角形内部不得含其他顶点。初始化全部耳后，每次仅更新两个相邻顶点，可$\mathcal{O}(n^{2})$；每轮重判全部顶点可退化为$\mathcal{O}(n^{3})$。
- 对凹多边形必须先判断对角线位于内部，退化共线点与边界点要统一处理。

## Delaunay 三角剖分

- 三角形的外接圆内部不含其他输入点，称满足空圆条件。无四点共圆时三角剖分唯一；共圆时可有多解。
- DT 最大化最小角，且最小生成树边与欧氏最近点对边包含在其中（适当处理退化）。
- 局部四边形凸时，可翻转对角线恢复空圆条件。非凸四边形不能直接翻边。

[C++ 源码](../templates/06-geometry/incircle.cpp)

- 当 ABC 逆时针，incircle>0 表示 D 在外接圆内。ABC 顺时针时符号反转，三点共线时无唯一外接圆。

## 两条构造路线

```text
Bowyer-Watson:
    start with a super-triangle that strictly contains all points
    insert a point and locate triangles whose circumcircles contain it
    remove those triangles; extract the cavity boundary
    connect the boundary to the new point
    finally remove every triangle incident to a super-triangle vertex
Divide and conquer:
    split by x, triangulate both halves
    locate the lower common tangent
    merge upward while deleting edges that violate the empty-circle test
```

- 随机增量配合点定位、冲突维护可达到期望$\mathcal{O}(n \log  n)$。每次暴力遍历全部三角形找洞会退化到$\mathcal{O}(n^{2})$。
- 重复点先去重，全共线单独输出线结构。共圆用一致的符号扰动或编号规则破平局，防止反复翻边。

## Voronoi 图

- 每个点的区域是离它不比其他站点更远的位置，边在两站点的垂直平分线上。普通欧氏点 Voronoi 单元为凸集。
- 与 DT 对偶：Delaunay 三角形外心是 Voronoi 顶点，相邻三角形外心相连得 Voronoi 边，凸包外侧对应无界射线。
- 有界窗口内可按各对站点的“更近一侧”半平面裁剪。全部单元共$\mathcal{O}(n^{2})$个约束，逐次裁剪的总时间还取决于中间顶点数。

# 计算几何 · 反演变换

## 定义与基本公式

反演中心 O、正参数$k=R^{2}$，P≠O 的像 $P'=O+\frac{k(P-O)}{|P-O|^2}$。满足 OP·OP'=k，连续做两次回到原点。

[C++ 源码](../templates/06-geometry/inversion.cpp)

- O 对应无穷远点，图形经过 O 时像可能从圆变成直线。不要将 O 本身送入除法公式。
- 距离满足 $|P'Q'|=\frac{k|PQ|}{|OP||OQ|}$。反演保角的绝对值，但反向，不保持长度、面积或内外关系。

## 圆、直线的像

- 圆心 C、半径 r，令 c=C-O、$\delta =|c|^{2}-r^{2}$。$\delta \ne 0$时像仍为圆，中心 $O+\frac{k c}{\delta}$，半径 $\frac{kr}{|\delta|}$。
- $\delta =0$时圆经过 O，像为直线 c·(X'-O)=k/2。不能继续使用除以$\delta $的圆公式。
- 直线过 O 时像仍在原直线。平移 O 为原点，直线 n·X=d、d≠0 的像为经过 O 的圆。
- 该圆中心 $\frac{k n}{2d}$，半径 $\frac{k|n|}{2|d|}$。圆心位置依 d 的符号变化。
- $\delta <0$表示 O 在原圆内，圆内区域在像中变为圆外；只转换边界不会自动保留可行侧。

## 切线与 Apollonius 型问题

- 圆或线在非 O 处相切，反演后仍相切。选择相交两圆的交点为中心，可将它们化为两条相交直线。
- 求与给定两圆、一直线相切的圆时，可将部分约束化简，枚举内切/外切的有向半径条件，再反演回去核验。
- 与两条相交直线相切的圆心位于两条角平分线上。与平行线相切的圆心在中线，半径为线距的一半。
- 反演后的线可能对应原来经过 O 的圆，也可能得到不符合原题半径、位置条件的候选，必须回到原坐标检查。
- 多次变换时分别保存中心、k、方向与内外关系。候选点接近 O 时误差被放大，需提高精度。

## 几何录入前检查

- 点方向与多边形朝向是否统一，边界是否算入，输出是否允许退化点、线段、零半径圆。
- 排序比较器是否严格，平方根/反三角函数是否做合法域修正，重复点是否引入零分母。
- 整数差、叉积、点积、面积累加是否在所选类型内，long double 是否符合评测精度。

# 计算几何 · 手写有理数

分数始终约为最简形式，分母为正，零统一为 0/1。分子和分母存 ll，中间运算用 i128，约分用 GNU 的 std::__gcd，传入同类型非负整数。类型别名见第 13 页。

[C++ 源码](../templates/06-geometry/rational.cpp)

- 比较用宽整数交叉乘，无需 eps。可用于 sort、set 和 pair 坐标的字典序。
- 直线 $y=x$ 与 $x+2y=1$ 的交点为 x=y=Rat(1,3)。一般直线交点可用克拉默法则，行列式为 0 时先判平行或重合。
- 构造时传 ll 范围内的分子和非零分母，除数须非零。每一步约分后的分子和正分母都须能用 ll 表示。
- 输入十进制坐标时，从字符构造分子和 10 的幂作为分母，避免先转浮点。输出读 p 和 q。
