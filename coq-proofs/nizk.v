(* NIZK Proof System - verification script by Remi Bazin *)

Load utils.




(* Primery group order *)
Variable p : nat.

Hypothesis p_big : 2 <= p.




(* Bilinear groups and a few constants *)
Inductive Fp : Set :=
  | ConstrFp : forall x:nat, (x<p -> Fp).

Inductive G1 : Set :=
  | ConstrG1 : Fp -> G1.
Inductive G2 : Set :=
  | ConstrG2 : Fp -> G2.
Inductive GT : Set :=
  | ConstrGT : Fp -> GT.

Inductive B1 : Set :=
  | ConstrB1 : G1 -> G1 -> B1.
Inductive B2 : Set :=
  | ConstrB2 : G2 -> G2 -> B2.
Inductive BT : Set :=
  | ConstrBT : GT -> GT -> GT -> GT -> BT.

Definition Fp_0 : Fp :=
  ConstrFp 0 (le_transitive 1 2 p (conj (le_S 1 1 (le_n 1)) p_big)).
Definition Fp_1 : Fp := ConstrFp 1 p_big.

Definition G1_O : G1 := ConstrG1 Fp_0.
Definition G2_O : G2 := ConstrG2 Fp_0.
Definition GT_1 : GT := ConstrGT Fp_0.
Definition B1_O : B1 := ConstrB1 G1_O G1_O.
Definition B2_O : B2 := ConstrB2 G2_O G2_O.
Definition BT_1 : BT := ConstrBT GT_1 GT_1 GT_1 GT_1.




(* Group operations *)
(* TODO
Definition addFp (a b:Fp) : Fp :=
  match a with | ConstrFp v_a p_a =>
    match b with | ConstrFp v_b p_b =>

    end
  end
.
*)

Variable addFp : Fp -> Fp -> Fp.
Variable addG1 : G1 -> G1 -> G1.
Variable addG2 : G2 -> G2 -> G2.
Variable multGT : GT -> GT -> GT.

Definition addB1 (a b:B1) : B1 :=
  match a with | ConstrB1 a1 a2 =>
    match b with | ConstrB1 b1 b2 =>
      ConstrB1 (addG1 a1 b1) (addG1 a2 b2)
    end
  end
.

Definition addB2 (a b:B2) : B2 :=
  match a with | ConstrB2 a1 a2 =>
    match b with | ConstrB2 b1 b2 =>
      ConstrB2 (addG2 a1 b1) (addG2 a2 b2)
    end
  end
.

Definition multBT (a b:BT) : BT :=
  match a with | ConstrBT a1 a2 a3 a4 =>
    match b with | ConstrBT b1 b2 b3 b4 =>
      ConstrBT (multGT a1 b1) (multGT a2 b2) (multGT a3 b3) (multGT a4 b4)
    end
  end
.




(* Identity element *)
Definition Is_zero (G:Type) (add:G->G->G) (e:G) : Prop :=
  forall a:G, ((add e a) = a)
.

(* Definition of an abelian group that is isomorphic to Fp *)
Definition Is_Fp_isomorphic (G:Type) (add:G->G->G) : Prop :=
  (* Note: Closure comes from the type of add. *)
  (* Associativity *)
  (forall (a:G) (b:G) (c:G), ((add a (add b c)) = (add (add a b) c)))
  /\
  (* Commutativity *)
  (forall (a:G) (b:G), ((add a b) = (add b a)))
  /\
  (* Identity element *)
  (exists e:G, (Is_zero G add e))
  /\
  (* Inverse element *)
  (forall a:G, exists b:G, exists e:G, ((Is_zero G add e) /\ ((add a b) = e)))
  /\
  (* Order at most p *)
  (*forall a:G, exists *)True
.

(* Only abelian groups are used *)
Hypothesis Fp_abelian : (Is_Fp_isomorphic Fp addFp).
Hypothesis G1_abelian : (Is_Fp_isomorphic G1 addG1).
Hypothesis G2_abelian : (Is_Fp_isomorphic G2 addG2).
Hypothesis GT_abelian : (Is_Fp_isomorphic GT multGT).
