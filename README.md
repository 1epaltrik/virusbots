<p style="text-align:center;"><img src="https://openedtech.ellak.gr/wp-content/uploads/sites/31/2021/10/virusbots-logo.png" alt="" width="171" height="165" ></p>
<p style="text-align: justify;">Το έργο μας αποτελείται από διάφορες τεχνολογίες οι οποίες όλες μαζί αποτελούν ένα σύστημα καταγραφής βεβαρημένης ατμόσφαιρας, που μπορεί να συμβάλλει στην παρακολούθηση του ιικού φορτίου στις σχολικές αίθουσες. Συγκεκριμένα οι επιμέρους λειτουργίες του είναι:</p>

<ul>
 	<li><strong> </strong><strong>Καταμέτρηση ατόμων</strong></li>
</ul>

<p style="text-align: justify; padding-left: 40px;">Καταγράφει πόσοι μαθητές βρίσκονται σε κάθε αίθουσα την κάθε στιγμή (ουσιαστικά χρησιμοποιείται ως παρουσιολόγιο). Χρησιμοποιήσαμε δύο Αισθητήρες Απόστασης Υπερήχων οι οποίοι συνδέονται σε ένα Arduino Mega. Ο Α αισθητήρας τοποθετήθηκε στην έξω πλευρά της τάξης ενώ ο Β στην μέσα πλευρά. Εάν ο Α αισθητήρας ενεργοποιηθεί πριν από τον Β σημαίνει ότι ένα άτομο εισήλθε μέσα στην αίθουσα. Αντίστοιχα εάν ενεργοποιηθεί πρώτα ο Β καταλαβαίνουμε πως ένα άτομο εξήλθε από την τάξη.</p>

<ul>
 	<li><strong>Έλεγχος αν η αίθουσα αερίζεται</strong></li>
</ul>
<p style="text-align: justify; padding-left: 40px;">Για να καταγράψουμε εάν η αίθουσα αερίζεται ελέγχουμε εάν ένα παράθυρο είναι ανοιχτό ή όχι. Η καταγραφή έγινε με την χρήση ενός μαγνητικού αισθητήρα (Magnetic Reed Switch), που συνδέεται σε ένα Arduino Mega. Ο μαγνητικός αισθητήρας αποτελείται από δύο μέρη: το ένα τοποθετείται στο κούφωμα ενώ το άλλο αντίστοιχα στο παράθυρο, έτσι ώστε να είναι σε επαφή. Όταν το παράθυρο ανοίξει, χάνεται η επαφή. Επομένως καταλαβαίνουμε πως το παράθυρο είναι ανοιχτό και η αίθουσα αερίζεται.</p>
&nbsp;
<ul>
 	<li><strong>Καταγραφή ποιότητας και επιπέδου ρύπανσης αέρα</strong></li>
</ul>

<p style="text-align: justify; padding-left: 40px;">Για την καταγραφή της <strong>ποιότητας και του επιπέδου ρύπανσης αέρα </strong> χρησιμοποιήθηκαν δύο (2) αισθητήρες. Για την μέτρηση:</p>

<ol>
 	<li style="list-style-type: none;">
<ol>
 	<li style="text-align: justify;">Της ποιότητας του αέρα χρησιμοποιήσαμε έναν αισθητήρα Ποιότητας Αέρα (CCS811). Ο αισθητήρας καταγράφει τα επίπεδα διοξειδίου του άνθρακα (CO<sub>2</sub>) και πτητικών οργανικών ενώσεων (tVOC).</li>
 	<li style="text-align: justify;">Του επιπέδου ρύπανσης του αέρα χρησιμοποιήσαμε έναν Particulate Matter Sensor PMS5003 ο οποίος ανιχνεύει τα μικροσωματίδια που είναι στην ατμόσφαιρα της αίθουσας. Γίνεται καταμέτρηση δύο διαστάσεων μικροσωματιδίων, των PM2.5 και PM10.</li>
</ol>
</li>
</ol>
<ul>
 	<li><strong>Καταγραφή θερμοκρασίας και υγρασίας αιθουσών</strong></li>
</ul>
<img class="alignnone size-medium wp-image-6430 aligncenter" src="https://openedtech.ellak.gr/wp-content/uploads/sites/31/2021/10/case-300x271.png" alt="" width="300" height="271" />
<p style="text-align: justify;">Όλες οι τιμές που καταγράφονται με τα παραπάνω συστήματα αποστέλλονται μέσω Wi-Fi και του ESP8266 στο νέφος  και είναι προσβάσιμες μέσω διαδικτύου στην ιστοσελίδα <a href="http://www.thinkspeak.com">www.thinkspeak.com</a> στα 3 κανάλια μας <a href="https://thingspeak.com/channels/1507949/">https://thingspeak.com/channels/1507949/</a>, <a href="https://thingspeak.com/channels/1312761/">https://thingspeak.com/channels/1312761/</a>, <a href="https://thingspeak.com/channels/1507989/">https://thingspeak.com/channels/1507989/</a> καθώς χρησιμοποιήθηκε η τεχνολογία Internet of Things (IoT). Οι τιμές αυτές μπορούν να επεξεργασθούν προκειμένου να δώσουν στοιχεία για το κατά πόσο επηρεάζει η βεβαρημένη ατμόσφαιρα των αιθουσών (ανάλογα με τις συνθήκες) τις απουσίες των μαθητών. Το σύστημα καταγραφής βεβαρημένης ατμόσφαιρας τοποθετήθηκε σε 3 αίθουσες με διαφορετικό προσανατολισμό (βόρειο, νότιο και δυτικό αντίστοιχα), προκειμένου να είναι δυνατή η σύγκριση δεδομένων και η εξαγωγή χρήσιμων συμπερασμάτων.</p>
<p style="text-align: justify;">Η έρευνα που διεξήχθη, ο σχεδιασμός που έγινε, ο τρόπος που υλοποιήθηκαν οι διάφορες τεχνολογίες για την παρακολούθηση παραγόντων που μπορούν να επηρεάσουν το ιικό φορτίο στις σχολικές αίθουσες καθώς και το αναλυτικό εκπαιδευτικό υλικό που αναπτύχθηκε βρίσκονται στο αποθετήριο github. Σύνδεσμος github <a href="https://github.com/1epaltrik/virusbots">https://github.com/1epaltrik/virusbots </a></p>
<p style="text-align: justify;">Αναλυτικότερα, στον σύνδεσμο:</p>

<ul>
 	<li style="text-align: justify;"><a href="https://github.com/1epaltrik/virusbots/tree/master/docs">https://github.com/1epaltrik/virusbots/tree/master/docs</a> υπάρχει η έρευνα και η σχεδίαση-τρόπος υλοποίησης του έργου καθώς και το κόστος των υλικών του</li>
 	<li style="text-align: justify;"><a href="https://github.com/1epaltrik/virusbots/tree/master/docs/fylla_ergasias"><span style="color: #ff9900;">https://github.com/1epaltrik/virusbots/tree/master/docs/fylla_ergasias </span></a>υπάρχουν αναλυτικά φύλλα εργασίας για κάθε μέρος του έργου</li>
 	<li style="text-align: justify;"><span style="color: #ff9900;"><a href="https://github.com/1epaltrik/virusbots/tree/master/code">https://github.com/1epaltrik/virusbots/tree/master/code </a></span>υπάρχουν στους αντίστοιχους φακέλους τα αρχεία κώδικα Arduino (.ino), τα σχέδια Fritzing (.fzz) και τα τρισδιάστατα μοντέλα (.stl) του κουτιού που χρησιμοποιήθηκαν στο έργο</li>
</ul>
&nbsp;

Tο βίντεο με λεπτομέρειες του έργου, από την αρχή μέχρι και την ολοκλήρωσή του θα το βρείτε στην ιστοσελίδα: https://youtu.be/LB_R5pna_wI
