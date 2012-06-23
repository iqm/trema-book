= OpenFlow の仕組み

== コントロールとデータを分離する
=== 分離していないと？
=== コントローラの仕事

== OpenFlow プロトコル
=== 未知のパケット (packet-in)
=== フローを更新する (flowmod)

== フローの中身
=== マッチングルール

マッチングルールは、OpenFlow スイッチがパケットを受け取ったときにアクショ
ンを起こすための条件です。OpenFlow 1.0 では、@<table>{matching_rules}
の 12 種類の条件が使えます。これらの条件はイーサネットや TCP/UDP でよく
使われる値ですので、ネットワークに詳しい人のためにそれぞれがどのレイヤ
に相当するかも入れてあります。

//table[matching_rules][マッチングルールで指定できる 12 種類の条件]{
名前			説明				レイヤ
--------------------------------------------------------------
Ingress Port		スイッチの物理ポート番号	L1
Ether src		送信元 MAC アドレス		L2
Ether dst		宛先 MAC アドレス		L2
Ether type		イーサネットの種別		L2
VLAN id			VLAN IDBig Switch		L2
VLAN priority		VLAN PCP の値 (CoS)		L2
IP src			送信元 IP アドレス		L3
IP dst			宛先 IP アドレス		L3
IP proto		IP のプロトコル種別		L3
IP ToS bits		IP の ToS 情報			L3
TCP/UDP src port	TCP/UDP の送信元ポート番号	L4
TCP/UDP dst port	TCP/UDP の宛先ポート番号	L4
//}

従来のネットワーク機器では、ネットワークの各レイヤに対応した個々のネッ
トワーク機器が、そのレイヤで処理を行うのが当然でした。たとえばレイヤ 2
スイッチは宛先の MAC アドレスを用いてスイッチが転送先の物理ポートを特定
し、そのポートにパケットを送ります。レイヤ 3 スイッチであれば、宛先の
IP アドレスを使って同様の動きをします。

OpenFlow の世界では、マッチングルールで指定できる条件を自由に組み合わせ
て通信を制御します。つまり、レイヤや宛先のような従来の枠組みではなく、
「スイッチの物理ポート 1 番から届く、宛先が TCP 80 番 (= HTTP) のパケッ
トを書き換える」とか「MAC アドレスが xx:xx:xx:xx:xx:xx で宛先の IP アド
レスが 192.168.0.0/24 のものは遮断する」というふうに自由に条件を組み合
わせることができるのです。

===[column] 取間先生曰く: OSI ネットワークモデルが壊れる？

あるネットワークの経験豊富な若者がこんな事を言っていた「OpenFlow のよう
にレイヤをまたがって自由に何でもできるようになると、OSI ネットワークモ
デルが壊れるんじゃないか？」

その心配は無用じゃ。OSI ネットワークモデルは正確に言うと「OSI 参照モデ
ル」と言って、通信プロトコルを分類して見通しを良くするために定義した "
参照用" の階層モデルじゃ。たとえば自分が xyz プロトコルというのを作った
けど人に説明したいというときに、どう説明するか考えてみてほしい。「これ
はレイヤ 3 のプロトコルで、…」という風に階層を指して (参照して) 説明を
始めれば相手に通りがいいじゃろ。つまり、OSI ネットワークモデルはネット
ワーク屋同士で通じる「語彙」として使える、まことに便利なものじゃ。

でも、これはあくまで「参照」であって「規約」ではないので、すべてのネッ
トワークプロトコル、ネットワーク機器がこれに従わなきゃならん、というも
のではないぞ。さっき言ったように「この ○○ は、仮に OSI で言うとレイヤ
4 にあたる」のように使うのが正しいのじゃ。

そして、OpenFlow はたまたまいくつものレイヤの情報が使える、ただそれだけ
のことじゃ。

===[/column]

=== アクション
=== 統計情報

== まとめ/参考文献
