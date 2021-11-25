void add(int element)
{
    if(this->count==0) {
        Node* temp = new Node(element);
        this->head=temp;
        this->tail=temp;
        this->count++;
        return;
    }
    Node* temp = new Node(element);
    tail->next=temp;
    temp->prev=tail;
    tail=temp;
    this->count++;
    return;
}

void add(int index, int element)
{
    if(index < 0 || index > this->count) throw out_of_range("");
    if(index==this->count || this->count ==0) {
        add(element);
        return;
    }
    else if(index==0) {
        Node* temp=new Node(element);
        head->prev=temp;
        temp->next=head;
        head=temp;
        this->count++;
        return;
    }
    int i = 0;
    Node* newNode = new Node(element);
    Node* pre=this->head;
    while(pre) {
        if(i == index - 1) break;
        pre=pre->next;
        i++;
    }
    Node* curr=pre->next;
    newNode->next=curr;
    pre->next=newNode;
    newNode->prev=pre;
    curr->prev=newNode;
    this->count++;
    return;
}

int removeAt(int index)
{
    if(index < 0 || index >= this->count) throw out_of_range("");
    if(this->count==0) return -1;
    if(this->count==1) {
        int t = head->value;
        delete this->head;
        this->head=NULL;
        this->tail=NULL;
        this->count=0;
        return t;
    }
    if(index == 0) {
        Node* temp = head;
        head = head->next;
        temp->next=NULL;
        head->prev=NULL;
        int t = temp->value;
        delete temp;
        this->count--;
        return t;
    }
    else if(index == this->count-1) {
        Node* temp = tail->prev;
        temp->next=NULL;
        tail->prev=NULL;
        int t = tail->value;
        delete tail;
        tail=temp;
        this->count--;
        return t;
    }
    Node* pre = this->head;
    int i=0;
    while(pre){
        if(i == index -1) break;
        pre=pre->next;
        i++;
    }
    Node* curr = pre->next;
    pre->next=curr->next;
    curr->next->prev=pre;
    int t = curr->value;
    curr->next=NULL;
    curr->prev=NULL;
    delete curr;
    this->count--;
    return t;
}

bool removeItem(int element)
{
    if(this->count==0) return false;
    int index = 0;
    Node* temp = this->head;
    while(temp) {
        if(temp->value==element) {
            removeAt(index);
            return true;
        }
        temp=temp->next;
        index++;
    }
    return false;
}

int get(int index)
{
    if(index < 0 || index >= this->count) throw out_of_range("");
    if(this->count==0) return -1;
    Node* temp = this->head;
    Node* temp2= this->tail;
    int i1 = 0 ;
    int i2 = this->count-1;
    while(temp && temp2){
        if(i1 == index) return temp->value;
        else if(i2 == index) return temp2->value;
        else {
            temp=temp->next;
            temp2=temp2->prev;
            i2--;
            i1++;
        }
    }
    return -1;
}

void set(int index, int element)
{
    if(index < 0 || index >=this->count) throw out_of_range("");
    if(this->count==0)return;
    int i=0;
    Node* temp = this->head;
    while(temp){
        if(index==i) {
            temp->value=element;
            return;
        }
        i++;
        temp=temp->next;
    }
    return;
}

int indexOf(int element)
{
    if(this->count==0) return -1;
    int index = 0;
    Node* temp = this->head;
    while(temp) {
        if(temp->value == element) return index;
        index++;
        temp=temp->next;
    }
    return -1;
}

bool contains(int element)
{
    if(indexOf(element) != -1) return true;
    return false;
}

int size()
{
    return this->count;
}

bool empty()
{
    return this->count==0;
}

void clear() 
{
    if(head==NULL) return;
    while(head != NULL) {
        Node* p = head;
        if(head==tail){
            delete p;
            break;
        }
        head=head->next;
        p->next = NULL;
        head->prev=NULL;
        delete p;
    }
    this->head = NULL;
    this->tail = NULL;
    this->count=0;
}
