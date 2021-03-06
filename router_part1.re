= ルータ (前編)

@<chap>{learning_switch} と @<chap>{traffic_monitor} では、L2 スイッチ機能を OpenFlow を使って実現する方法について見てきました。L2 スイッチにおける、パケットの MAC アドレスを見て転送するという動作の実現を行いました。しかし、@<chap>{openflow} で説明したように、OpenFlow でマッチングルールとして扱えるのはレイヤー 2 の情報だけではありません。

本章では、レイヤー 3 の情報を使うルータを OpenFlow で実現する方法を説明します。

== ルータとは

ルータと L2 スイッチは何が違うのでしょうか？パケットを転送する際に見るところが違うだけでしょうか？もちろん、パケットを転送する際に L2 スイッチは宛先 MAC アドレスを、ルータは宛先 IP アドレスを参照しており、それぞれ異なっています。それでは、なぜわざわざ異なるフィールドを参照する必要があるのでしょうか？その理由について、考えてみましょう。

=== L2 ネットワーク同士を接続する

OpenFlow の世界では、L2 といえば、Ethernet です。@<chap>{openflow} で説明したマッチングルールには、MAC アドレスや VLAN ID など、Ethernet のヘッダ中に含まれる情報が採用されています。この Ethernet は、現在最もメジャーな L2 技術といっても間違いではないでしょう。データセンター内で、各サーバ間をつなぐネットワークから、家庭内ネットワークまで Ethernet は、いろいろなところで使われています。

Ethernet の他に L2 技術はないのでしょうか？そんなことはありません。身近なところでは、PPP  (Point to Point Protocl) があります。以前 Internet を利用する際のダイアルアップ接続では、この PPP が用いられていました。また現在では ADSL や光ファイバーによるインターネット接続サービスにて PPP が用いられています。他にも、徐々に利用が減りつつありますが、ATM (Asynchronous Transfer Mode) やフレームリレーも L2 技術の一種です。今ほど Ethernet 技術が普及する前には、大学の計算機室のワークステーション間接続には FDDI (Fiber-Distributed Data Interface) という L2 技術が広く使われていました。

異なる L2 技術を採用したホスト間で通信を行うためには、一工夫が必要です。まず、L2 技術が異なれば、通信を行う相手を指定する方法に違いがあるためです。そこで、必要となるのが、L2 技術が異なっていても通信ができるよう共通言語としての L3 技術、Internet Protocol です。この IP を用いて、異なる L2 ネットワーク間の中継を行うのが、ルータの役割です。

=== Ethernet が普及したらルータはいらなくなる？

様々な L2 技術があることを紹介しましたが、OpenFlow で扱うことが出来るのは、そのうちでも Ethernet だけです。L2 技術がすべて Ethernet になれば、ルータはいらなくなるのでしょうか？ルータは、Ethernet でできた複数の L2 ネットワーク同士をつなぐ用途でも実際に用いられています。

しかし Ethernet だけで大きなネットワークを作るのには、以下のような問題があります。

 * ブロードキャストの問題
 * セキュリティ上の問題

Ethernet だけでネットワークを作る場合にも、いくつかの比較的小さな L2 ネットワークを作り、それらをルータでつなぐといった構成は、一般的に用いられています。

== ルータの動作



== まとめ/参考文献
