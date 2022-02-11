# Bolt

1. etcd 使用 boltdb 做存储引擎，读性能优秀，写性能一般，稳定
2. boltdb 的物理存储单元是 page，一般为 4K
3. page 有不同的类型
   - meta: 存储 page 的元信息
   - branch: B+树干
   - leaf: B+树叶子节点，保存数据
   - freelist: 存储空闲可用的 pageID
4. boltdb 使用 cow 的方式，保证无覆盖写，等到 commit 的时候，最终修改引用，从而切换整个 db 的路径
5. meta 的 page 有两个，通过轮转写来实现的无覆盖写
6. B+树相比 LSM Tree 天然有随机读的性能优势，树高度稳定，boltdb 通过 mmap 把文件映射到内存，读的缓存交给了操作系统的 page cache
7. boltdb 的写性能可能很差，只要改动一点点，就都会导致这个节点到 root 节点整个链路的更新
