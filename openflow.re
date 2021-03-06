= OpenFlow の仕組み

== コントロールとデータを分離する
=== 分離していないと？
=== コントローラの仕事

== OpenFlow プロトコル
=== 未知のパケット (packet-in)
=== フローを更新する (flowmod)

== フローの中身
=== マッチングルール

フローの構成要素の一つにマッチングルールがあります。マッチングルールとは、OpenFlow スイッチがパケットを受け取ったときにアクションを起こすかどうかを決める条件です。たとえば「パケットの宛先が http サーバだったら」とか「パケットの送信元がローカルネットワークだったら」などという条件に適合したパケットにだけ、スイッチがアクションを起こすというわけです。

OpenFlow 1.0 では、@<table>{matching_rules} の 12 種類の条件が使えます。これらの条件はイーサネットや TCP/UDP でよく使われる値です。これらの条件をネットワークレイヤの視点で整理すると、レイヤ 1 からレイヤ 4 までの情報を使えることがわかります。

//table[matching_rules][マッチングルールで指定できる 12 種類の条件]{
名前			説明				レイヤ
--------------------------------------------------------------
Ingress Port		スイッチの物理ポート番号	L1
Ether src		送信元 MAC アドレス		L2
Ether dst		宛先 MAC アドレス		L2
Ether type		イーサネットの種別		L2
VLAN id			VLAN ID				L2
VLAN priority		VLAN PCP の値 (CoS)		L2
IP src			送信元 IP アドレス		L3
IP dst			宛先 IP アドレス		L3
IP proto		IP のプロトコル種別		L3
IP ToS bits		IP の ToS 情報			L3
TCP/UDP src port	TCP/UDP の送信元ポート番号	L4
TCP/UDP dst port	TCP/UDP の宛先ポート番号	L4
//}

OpenFlow の世界では、マッチングルールで指定できる条件を自由に組み合わせて通信を制御します。つまり、レイヤや宛先のような従来の枠組みではなく、たとえば、

 * スイッチの物理ポート 1 番から届く、宛先が TCP 80 番 (= HTTP) のパケットを書き換える
 * MAC アドレスが 02:27:e4:fd:a3:5d で宛先の IP アドレスが 192.168.0.0/24 のものは遮断する
   
というふうに自由に複数の条件を組み合わせることができるのです。

これに対して従来のネットワーク機器では、ネットワークの各レイヤに対応した個々のネットワーク機器が、そのレイヤで処理を行うのが当然でした。たとえばレイヤ 2 スイッチは宛先の MAC アドレスを用いてスイッチが転送先の物理ポートを特定し、そのポートにパケットを送ります。レイヤ 3 スイッチであれば、宛先の IP アドレスを使って同様の動きをします。こうした制限を無くし、ひとつのレイヤにとらわれない制御をするのが OpenFlow の特徴の一つと言えます。

===[column] 取間先生曰く: OSI ネットワークモデルが壊れる？

あるネットワークの経験豊富な若者がこんな事を言っていた「OpenFlow のようにレイヤをまたがって自由に何でもできるようになると、OSI ネットワークモデルが壊れるんじゃないか？」

その心配は無用じゃ。OSI ネットワークモデルは正確に言うと「OSI 参照モデル」と言って、通信プロトコルを分類して見通しを良くするために定義した "参照用" の階層モデルじゃ。たとえば自分が xyz プロトコルというのを作ったけど人に説明したいというときに、どう説明するか考えてみてほしい。「これはレイヤ 3 のプロトコルで、…」という風に階層を指して (参照して) 説明を始めれば相手に通りがいいじゃろ。つまり、OSI ネットワークモデルはネットワーク屋同士で通じる「語彙」として使える、まことに便利なものじゃ。

でも、これはあくまで「参照」であって「規約」ではないので、すべてのネットワークプロトコル、ネットワーク機器がこれに従わなきゃならん、というものではないぞ。さっき言ったように「この ○○ は、仮に OSI で言うとレイヤ4 にあたる」のように使うのが正しいのじゃ。

そして、OpenFlow はたまたまいくつものレイヤの情報が使える、ただそれだけのことじゃ。

===[/column]

=== 統計情報

フローの構成要素の一つに統計情報があります。統計情報はスイッチに実装されたカウンタの一種で、パケットの転送数やエラー数などを数えてくれます。コントローラはこのカウンタの値を使うことによってたとえば、

 * ネットワーク情報の可視化
 * エラーの通知や警告、予測
 * トラフィックの大小に応じたユーザごとの帯域制御
 * トラフィックの傾向に応じた転送パスの切り替えやスイッチの ON/OFF 制御

などを行うことができます。

==== 統計情報の利用例

ひとつ具体的な実装例をお見せしましょう。Trema のサンプルアプリケーションのひとつ、トラフィックモニタがこの統計情報を利用しています。これは、普通のレイヤ 2 スイッチとして動作しますが、ユーザごと (MAC アドレスごと) の転送パケット数とバイト数を統計情報を利用して表示してくれます。

トラフィックモニタの実行例は次のようになります。パケットを送信したホストの MAC  アドレスごとに、スイッチが転送したパケットの数とバイト数を定期的に更新して表示します。

//emlist{
% cd src/examples/traffic_monitor/
% trema run ./traffic-monitor.rb -c ./traffic-monitor.conf
……
00:00:00:00:00:01 10 packets (640 bytes)
……
00:00:00:00:00:02 10 packets (640 bytes)
……
//}

トラフィックモニタの実装は第 II 部の @<chap>{traffic_monitor} で詳しく解説します。統計情報の利用例として参考にしてみてください。

==== カウンタの種類

OpenFlow 1.0 ではカウンタはスイッチの次の 4  箇所にセットされています。取得したい情報の種類や粒度に応じて使い分けることができます。

 * フローテーブルごと
 * フローごと
 * ポートごと
 * キューごと

少し細かいですが、以下にそれぞれで定義されている統計情報をまとめます。

===== フローテーブルごと

 * 有効なエントリの数
 * パケットのルックアップ数
 * パケットのマッチ数

===== フローごと

 * 受信パケット数
 * 受信バイト数
 * フローが作られてからの経過時間 (秒)
 * フローが作られてからの経過時間 (ナノ秒)

===== ポートごと

 * 受信パケット数
 * 転送パケット数
 * 受信バイト数
 * 転送バイト数
 * 受信ドロップ数
 * 転送ドロップ数
 * 受信エラー数
 * 転送エラー数
 * フレームアライメントエラー数
 * 受信オーバーランエラー数
 * 受信 CRC エラー数
 * コリジョン数

===== キューごと

 * 転送パケット数
 * 転送バイト数
 * 転送オーバーランエラー数

さて、この節ではフローの統計情報の中身と利用例を紹介しました。スイッチからのフィードバック情報である統計情報をうまく使うことによって、ネットワークの制御や可視化の精度を上げることができます。

=== アクション

フローの構成要素の一つにアクションがあります。アクションとは、スイッチに入ってきたパケットをどう料理するか、という@<em>{動詞}にあたる部分です。よく「OpenFlow でパケットを書き換えて曲げる」などと言いますが、これらはすべてこのアクションで実現できることです。それでは、OpenFlow 1.0 ではどんなアクションが定義されているか見てみましょう。

==== アクションの種類

アクションは大きく分けて次の 4 種類があります。

 * Forward: パケットを指定したポートから出力する
 * Modify-Field: パケットの中身を書き換える
 * Drop: パケットを捨てる
 * Enqueue: ポートごとに指定されたスイッチのキューに入れる。QoS 用。
 
アクションは動詞と同じく指定した順番に実行されます。「ごはんを作って、食べて、片付ける」といったふうに。たとえば、パケットを書き換えて指定したポートに出したいときには、

//emlist{
[Modify-Field, Forward]
//}

というアクションのリストを指定します。ここで、アクションは指定された順番に実行されることに注意してください。アクションの順番を変えてしまうと、違う結果が起こります。たとえば「ごはんを食べてから、ごはんを作る」と最後にごはんが残ってしまいます。同様に先ほどの例を逆にしてしまうと、先にパケットがフォワードされてしまうので Modify-Field は実行されなくなります。

//emlist{
# パケットを書き換える前にフォワードされてしまう。
[Forward, Modify-Field]
//}

同じ動詞を複数指定することもできます。

//emlist{
[Modify-Field A, Modify-Field B, Forward A, Forward B]
//}

この場合は、フィールド A と B を書き換えて、ポート A と B へフォワードする、と読めます。このように、複数の箇所を書き換えたり複数のポートからパケットを出したい場合には、アクションを複数連ねて指定します。

Drop は特殊なアクションで、実際には Drop アクションというものが具体的に定義されているわけではありません。アクションのリストに Forward アクションをひとつも入れなかった場合、そのパケットはどこにもフォワードされずに捨てられます。これを Drop アクションと呼んでいるわけです。

それでは、最もよく使われる Forward アクションと Modify-Field アクションで具体的に何が指定できるか見て行きましょう。

===== Forward アクション

Forward アクションでは指定した出力ポートにパケットを転送します。出力ポートはポート番号で指定することもできますが、いくつか論理的なポートが定義されています。

 * ポート番号: パケットを指定した番号のポートに出す。
 * IN_PORT: パケットを入ってきたポートに出す。
 * ALL: パケットを入ってきたポート以外のすべてのポートに出す。
 * FLOOD: パケットをスイッチの作ったスパニングツリーに沿って出す。スイッチが複数台ある場合にはループが起こらないので ALL よりも安全。
 * CONTROLLER: パケットをコントローラに明示的に送り、Packet In を起こす。
 * NORMAL: パケットをスイッチの機能を使って転送する。従来のスイッチの動作と同じ。
 * LOCAL: パケットをスイッチのローカルポートに出す。ローカルポートを監視するアプリケーションに特殊な処理をさせたい場合に使う。あまり使われない。

===== Modify-Field アクション

Modify-Field アクションではパケットの様々なフィールドを書き換えることができます。書き換えできる項目は次のとおりです。

 * VLAN ID: 指定された VLAN ID をセットする、または既存のものがあれば書き換える。
 * VLAN プライオリティ: 指定された VLAN プライオリティをセットする、または既存のものがあれば書き換える。VLAN ID はゼロになる。
 * VLAN ヘッダ: VLAN のヘッダを除去する。
 * 送信元 MAC アドレス: 送信元の MAC アドレスを書き換える。
 * 宛先 MAC アドレス: 宛先の MAC アドレスを書き換える。
 * 送信元 IP アドレス: 送信元の IP アドレスを書き換える。
 * 宛先 IP アドレス: 宛先の IP アドレスを書き換える。
 * TCP/UDP 送信元ポート: TCP/UDP の送信元ポート番号を書き換える。
 * TCP/UDP 宛先ポート: TCP/UDP の宛先ポート番号を書き換える。
 * IP ToS bits: IP の ToS フィールドを書き換える。

さて、この節では OpenFlow で使えるアクションを見てきました。どんなアクションが使えるかを押さえておくことで、届いたパケットを自由自在に処理できるようになります。

== まとめ/参考文献
