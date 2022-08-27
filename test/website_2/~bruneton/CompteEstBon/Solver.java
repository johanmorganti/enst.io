package CompteEstBon;
import java.util.*;

final class ChoiceGenerator {

  public int[] currentChoice = new int[Solver.PMAX + 1];

  private int n,nCk,k,l;
  private boolean symetrical;
 
  void init (int n, boolean symetrical) {
  
    nCk = 1;     // = n!/(k!(n-k)!)
    k = 0;
    l = 1;
    this.n = n;
    this.symetrical = symetrical;
  }
  
  void nextChoice () {
  
    ++l;
    if (l > nCk) {
      nCk = (nCk*(n - k))/(k + 1);
      k = k + 1;
      l = 1;
      if (symetrical && 2*k == n)
        nCk = nCk/2;
    }

    if (l == 1) {
      currentChoice[0] = k;
      for (int i = 1; i <= k; ++i)
        currentChoice[i] = i;
    } else {
      int i = n;
      int p = k;
      boolean ok = false;
      
      do {
        if (currentChoice[p] < i) {
          ++currentChoice[p];
          for (int j = p + 1; j <= k; ++j)
            currentChoice[j] = currentChoice[j - 1] + 1;
          ok = true;
        } else {
          --p;
          --i;
        }
      } while (!ok && p != 0);
    }
  }
  
  boolean moreChoice () {
  
    if (symetrical)
      return (k != n/2) || (l != nCk);
    else
      return (k != n) || (l != nCk);
  }
}

final class SharingGenerator {

  public int[] nbs1 = new int[Solver.PMAX + 1];
  public int[] nbs2 = new int[Solver.PMAX + 1];
  
  private ChoiceGenerator generator = new ChoiceGenerator();
  private int[] nbs;
      
  void init (int[] nbs) {

    this.nbs = nbs;
    generator.init(nbs[0],true);
  }
  
  void nextSharing () {
      
    generator.nextChoice();
    int[] choice = generator.currentChoice;

    int currentIndex = 1;
    nbs1[0] = nbs2[0] = 0;
    
    for (int i = 1; i <= nbs[0]; ++i)
      if (currentIndex <= choice[0] && i == choice[currentIndex]) {
        ++currentIndex;
        nbs1[++nbs1[0]] = nbs[i];
      } else
        nbs2[++nbs2[0]] = nbs[i];
  }
  
  boolean moreSharing () {
  
    return generator.moreChoice();
  }
}

interface TreeVisitor {
 
  void visitNode (int n);
  void visitLeaf (int l);
}

final class Tree {

  private static final int EMPTY_NODE = 0;
  private int[] data = new int[Solver.STRMAX];
  private int size;

  Tree () {
  
    size = 1;
    data[0] = EMPTY_NODE;
  }

  void clone (Tree model) {
  
    size = model.size;
    System.arraycopy(model.data,0,data,0,size);
  }

  Tree insertNode (Tree model, int node) {
  
    int offset = model.firstEmptyNode() + 1;
    System.arraycopy(model.data,0,data,0,offset);
    System.arraycopy(model.data,offset,data,offset + 2,model.size - offset);
    data[offset++] = EMPTY_NODE;
    data[offset] = -node;
    size = model.size + 2;
    return this;
  }

  Tree insertLeaf (Tree model, int leaf) {
  
    clone(model);
    data[model.firstEmptyNode()] = leaf;
    return this;
  }

  void acceptVisitor (TreeVisitor v) {
  
    for (int i = 0; i < size; ++i)
      if (data[i] < 0)
        v.visitNode(-data[i]);
      else
        v.visitLeaf(data[i]);
  }
  
  private int firstEmptyNode () {
  
    int i = 0;
    while (data[i] != EMPTY_NODE)
      ++i;
    return i;
  }
}

class DoneException extends Exception { }

interface TreeConsumer {
 
  void eval (Tree t) throws DoneException;
}

final class TreeGenerator {

  private TreeConsumer consumer;

  private ChoiceGenerator choiceGen = new ChoiceGenerator();
  private Tree root = new Tree();
  
  private Stack lists = new Stack();
  private Tree[] treeStack = new Tree[Solver.STRMAX];
  private SharingGenerator[] sharingGenStack = new SharingGenerator[Solver.STRMAX];
  private int treeStackTop, sharingGenStackTop;
  
  TreeGenerator () {
  
    for (int i = 0; i < Solver.STRMAX; ++i) {
      treeStack[i] = new Tree();
      sharingGenStack[i] = new SharingGenerator();
    }
  }

  void generate (int n, TreeConsumer consumer) throws DoneException {
  
    this.consumer = consumer;
    lists.removeAllElements();
    lists.push(choiceGen.currentChoice);
    treeStackTop = sharingGenStackTop = 0;
    
    choiceGen.init(n,false);
    while (choiceGen.moreChoice()) {
      choiceGen.nextChoice();
      generateTrees(root);
    }
    
    lists.pop();
  }
  
  private void generateTrees (Tree t) throws DoneException {
  
    if (lists.size() == 0)
      consumer.eval(t);
    else {
      int[] nbs = (int[])lists.pop();
      Tree newT = treeStack[treeStackTop++];
      
      if (nbs[0] == 1)
        generateTrees(newT.insertLeaf(t,nbs[1]));
      else {
        SharingGenerator sharingGen = sharingGenStack[sharingGenStackTop++];
        lists.push(sharingGen.nbs2);
        lists.push(sharingGen.nbs1);
        
        sharingGen.init(nbs);
        while (sharingGen.moreSharing()) {
          sharingGen.nextSharing();
          generateTrees(newT.insertNode(t,1));
        }
        
        lists.pop();
        lists.pop();
        --sharingGenStackTop;
      }
      
      lists.push(nbs);
      --treeStackTop;
    }
  }
}

final class ArrayStack {

  private int[] values,bases,sizes;
  private int nbValues = 0;
  private int nbElements = 0;

  ArrayStack (int nbValues, int nbElements) {
  
    values = new int[nbValues];
    bases = new int[nbElements];
    sizes = new int[nbElements];
  }

  void flush () {
  
    nbElements = 0;
    nbValues = 0;
  }

  void push (int nb) {
  
    bases[nbElements] = nbValues;
    sizes[nbElements] = 1;
    values[nbValues] = nb;
    ++nbElements;
    ++nbValues;
  }

  void push (int[] nbs) {
  
    bases[nbElements] = nbValues;
    sizes[nbElements] = nbs[0];
    ++nbElements;
    System.arraycopy(nbs,1,values,nbValues,nbs[0]);
    nbValues += nbs[0];
  }
  
  void pop (int tab[]) {
  
    --nbElements;
    int size = sizes[nbElements];
    int offset = bases[nbElements] - 1;
    nbValues -= size;
    tab[0] = size;
    System.arraycopy(values,bases[nbElements],tab,1,size);
  }
}

final class Evaluator implements TreeConsumer, TreeVisitor {
  
  private final static int VIDE = -1;
  
  private int[] nbs;
  private int goal,precision;
  
  private ArrayStack evalStack = new ArrayStack(Solver.PMAX4 + 1,Solver.PMAX);
  private int[] aVal = new int[Solver.PMAX4 + 1];
  private int[] bVal = new int[Solver.PMAX4 + 1];
  private int[] result = new int[Solver.PMAX4 + 1];
  private SolutionStorage solution = new SolutionStorage();
  
  void init (int[] nbs, int goal) {
    
    this.nbs = nbs;
    this.goal = goal;
    precision = Math.abs(nbs[1] - goal);
    solution.init(nbs,goal);
  }
  
  Vector getBestSolution () {
  
    return solution.getSolution();
  }
  
  public void eval (Tree t) throws DoneException {
  
    evalStack.flush();
    t.acceptVisitor(this);
    evalStack.pop(result);
    
    for (int i = 1; i <= result[0]; ++i)
      if (Math.abs(result[i] - goal) < precision) {
        precision = Math.abs(result[i] - goal);
        solution.setSolution(t,i - 1);
      }

    if (precision == 0)
      throw new DoneException();
  }
  
  public void visitNode (int n) {
  
    evalStack.pop(bVal);
    evalStack.pop(aVal);
    
    int bm = bVal[0];
    int am = aVal[0];

    int i,j,a,b,k = 0;	  
    result[k++] = 4*am*bm;
	  
    for (j = 1; j <= bm; ++j)
      if ((b = bVal[j]) != VIDE) 
        for (i = 1; i <= am; ++i)
          if ((a = aVal[i])!= VIDE)
            result[k++] = a+b;
          else
            result[k++] = VIDE;
        else
          for (i = 1; i <= am; ++i)
            result[k++] = VIDE;

    for (j = 1; j <= bm; ++j)
      if ((b = bVal[j]) != VIDE) 
        for (i = 1; i <= am; ++i)
          if ((a = aVal[i])!= VIDE)
            result[k++] = a*b;
          else
            result[k++] = VIDE;
        else
          for (i = 1; i <= am; ++i)
            result[k++] = VIDE;

    for (j = 1; j <= bm; ++j)
      if ((b = bVal[j]) != VIDE) 
        for (i = 1; i <= am; ++i)
          if ((a = aVal[i])!= VIDE)
            result[k++] = Math.abs(a-b);
          else
            result[k++] = VIDE;
        else
          for (i = 1; i <= am; ++i)
            result[k++] = VIDE;
  
    for (j = 1; j <= bm; ++j)
      if (((b = bVal[j]) != VIDE) && (b != 0))
        for (i = 1; i <= am; ++i)
          if (((a = aVal[i]) != VIDE) && (a != 0))
            if (a > b)
              if (a%b == 0)
                result[k++] = a/b;
              else
                result[k++] = VIDE;
            else if (b%a == 0)
              result[k++] = b/a;
            else
              result[k++] = VIDE;
          else
            result[k++] = VIDE;
      else
        for (i = 1; i <= am; ++i)
          result[k++] = VIDE;
    
    evalStack.push(result);
  }
  
  public void visitLeaf (int l) {
  
    evalStack.push(nbs[l]);
  }
}

final class SolutionStorage implements TreeVisitor {

  private int nbs[],goal,n;
  private Tree solution = new Tree();
  private Vector result = new Vector();
  private int[] stack = new int[Solver.PMAX];
  private int top; 
  
  void init (int[] nbs, int goal) {
  
    this.nbs = nbs;
    this.goal = goal;
    solution.insertLeaf(solution,1);
  }
  
  void setSolution (Tree t, int n) {
  
    solution.clone(t);
    this.n = n;
  }
  
  Vector getSolution () {
  
    top = -1;
    result.removeAllElements();
    solution.acceptVisitor(this);
    if (result.isEmpty())
      result.addElement(stack[0] + "=" + stack[0]);
    if (stack[0] != goal)
      result.insertElementAt("une meilleure approximation est:",0);
    return result;
  }
  
  public void visitNode (int i) {
  
    int a = Math.max(stack[top],stack[top-1]);
    int b = Math.min(stack[top],stack[top-1]);
    int c = 0;    
    switch (n%4) {
      case 0: c = a+b; result.addElement(a + "+" + b + "=" + c); break;
      case 1: c = a*b; result.addElement(a + "*" + b + "=" + c); break;
      case 2: c = a-b; result.addElement(a + "-" + b + "=" + c); break;
      case 3: c = a/b; result.addElement(a + "/" + b + "=" + c); break;
    }    
    stack[--top] = c;
    n = n/4;
  }
  
  public void visitLeaf (int i) {
  
    stack[++top] = nbs[i];
  }
}

public class Solver {
 
  public static final int PMAX = 6;
  
  static final int PMAX4 = 1 << (2*PMAX - 2), STRMAX = 2*PMAX;
  private TreeGenerator producer = new TreeGenerator();
  private Evaluator consumer = new Evaluator();

  public Vector solve (int[] numbers, int goal) {
  
    int nbs[] = new int[numbers.length + 1];
      
    nbs[0] = numbers.length;
    System.arraycopy(numbers,0,nbs,1,nbs[0]);
      
    try {
      consumer.init(nbs,goal);
      producer.generate(nbs[0],consumer);
    }
    catch (DoneException e) {
    }
    return consumer.getBestSolution();
  }
}
