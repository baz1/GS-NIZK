(* NIZK Proof System - Useful functions by Remi Bazin *)

(* Proof that (a <= 0) -> (a = 0) *)
Lemma le_zero :
  forall (a:nat),
  ((a <= 0) -> (a = 0))
.
Proof.
  intro a.
  destruct a.
    (* Case a=0 *)
    intro useless.
    exact (eq_refl 0).
    (* Case a>0 *)
    intro H.
    inversion H.
Qed.

(* Proof that <= is transitive *)
Lemma le_transitive :
  forall (a:nat) (b:nat) (c:nat), (
    (a <= b) /\ (b <= c) -> (a <= c)
  )
.
Proof.
  intros a b c.
  intro H.
  case H.
  intro H1.
  elim c.
    (* Case c=0 *)
    intro H2.
    rewrite (le_zero b H2) in H1.
    exact H1.
    (* Case c --> c+1 *)
    intro n.
    intro Hind.
    intro H2.
    inversion H2.
    rewrite H0 in H1.
    exact H1.
    exact (le_S a n (Hind H3)).
Qed.

(* Proof that 0<=x for all x *)
Lemma zero_min : forall (x:nat), 0 <= x.
Proof.
  intro x.
  elim x.
    (* Case x=0 *)
    exact (le_n 0).
    (* Case x --> x+1 *)
    intros n H.
    exact (le_S 0 n H).
Qed.

(* Proof that a<=b -> (S a)<=(S b) *)
Lemma le_translates : forall (a b:nat), (a <= b) -> ((S a) <= (S b)).
Proof.
  intros a b.
  elim b.
    (* Case b=0 *)
    intro H.
    rewrite (le_zero a H).
    constructor.
    (* Case b --> b+1 *)
    intros n H1 H2.
    inversion H2.
      (* Case a<=b *)
      constructor.
      (* Case a<b *)
      exact (le_S (S a) (S n) (H1 H0)).
Qed.

(* Proof that <= is a totally ordered relation (fine grained version) *)
Lemma le_tot_ordered : forall (a b:nat), (a<=b) \/ (a>b).
Proof.
  intro a.
  elim a.
    (* Case a=0 *)
    intro b.
    refine (or_introl _).
    elim b.
      (* Case b=0 *)
      exact (le_n 0).
      (* Case b --> b+1 *)
      intros n H.
      exact (le_S 0 n H).
    (* Case a --> a+1 *)
    intros n H b.
    case (H b).
      (* Case n<=b *)
      intro H1.
      case H1.
        (* Case n=b *)
        exact (or_intror (le_n (S n))).
        (* Case n<b *)
        intros m H2.
        exact (or_introl (le_translates n m H2)).
      (* Case n>b *)
      intro H1.
      exact (or_intror (le_S (S b) n H1)).
Qed.

(* Proof that n+0=n *)
Lemma plus_n_O : (forall n, n + O = n).
Proof.
  intros n.
  elim n.
    (* Case n=0 *)
    simpl.
    exact (eq_refl O).
    (* Case n --> n+1 *)
    intros n0.
    intros H1.
    simpl.
    rewrite H1.
    exact (eq_refl (S n0)).
Qed.

Lemma plus_vs_le : forall (a b:nat), a <= a + b.
Proof.
  admit.
Qed.

(* Proof that + commutes *)
Lemma plus_comm : (forall (n m:nat), n + m = m + n).
Proof.
  intros n.
  elim n.
    (* Case n=0 *)
    intros m.
    rewrite (plus_n_O m).
    simpl.
    exact (eq_refl m).
    (* Case n --> n+1 *)
    intros n0.
    intros lH.
    intros m.
    elim m.
      (* Case m=0 *)
      rewrite (plus_n_O (S n0)).
      simpl.
      exact (eq_refl (S n0)).
      (* Case m --> m+1 *)
      intros m0.
      intros lH2.
      simpl.
      rewrite <- lH2.
      rewrite (lH (S m0)).
      simpl.
      rewrite (lH m0).
      exact (eq_refl (S (S (m0 + n0)))).
Qed.

(* Proof that there is no x such that x<x *)
Lemma le_wrong : forall (x:nat), ~ (S x <= x).
Proof.
  admit.
Qed.

Lemma sub_lowers : forall (a b:nat), (a-b <= a).
Proof.
  admit.
Qed.

Lemma sub_zero : forall (a:nat), a-O=a.
Proof.
  intro a.
  destruct a.
    (* Case a=0 *)
    unfold minus.
    reflexivity.
    (* Case a>0 *)
    unfold minus.
    reflexivity.
Qed.

Lemma plus_1_vs_S : forall (a:nat), a+1=S a.
Proof.
  intro a.
  rewrite (plus_comm a 1).
  unfold plus.
  reflexivity.
Qed.

Lemma plus_transitive : forall (a b c:nat), a+b+c=a+(b+c).
Proof.
  admit.
Qed.

Lemma plus_funny_proof : forall (a b c:nat), a+b=b+c -> a=c.
Proof.
  admit.
Qed.

(* Function that checks the result of a modulo operation *)
Definition is_modulo (a n r:nat) : Prop :=
  (r<n) /\ (exists (k:nat), a=(k*n+r))
.

(* Theorem that proves the existence of a modulo operation *)
Theorem modulo_existence :
  forall (a n:nat), (1<=n) ->
  (exists r, is_modulo a n r).
Proof.
  intros a n H.
  elim a.
    (* Case a=0 *)
    refine (ex_intro _ 0 _).
    unfold is_modulo.
    assert (subg : 0 = 0 * n + 0).
    simpl.
    exact (eq_refl O).
    exact (conj H (ex_intro (fun (k:nat) => 0 = k * n + 0) 0 subg)).
    (* Case a --> a+1 *)
    unfold is_modulo.
    intros n0 H1.
    case H1.
    intros x H2.
    case H2.
    intros H21 H22.
    case H22.
    intros x0 H221.
    pose (test := (le_tot_ordered (S (S x)) n)).
    case test.
      (* Case (S x)<n *)
      intro H3. (* left *)
      assert (right : (S n0) = x0 * n + (S x)).
      simpl.
      rewrite (plus_comm (x0*n) (S x)).
      simpl.
      rewrite (plus_comm x (x0*n)).
      rewrite <- H221.
      exact (eq_refl (S n0)).
      exact (ex_intro _ (S x) (conj H3
        (ex_intro (fun (k:nat) => S n0 = k * n + (S x)) x0 right))).
      (* Case (S x)>=n *)
      intro H3.
      refine (ex_intro _ O _).
      refine (conj H _).
      refine (ex_intro _ (S x0) _).
      inversion H3.
      unfold mult.
      assert (subg : n0 = x + x0 * n + 0).
      admit.
      simpl.
      rewrite <- H4.
      unfold mult in subg.
      rewrite <- subg.
      reflexivity.
      pose (impossible1 := conj H21 (le_S_n n x H4)).
      unfold lt in impossible1.
      pose (impossible2 := le_wrong x (le_transitive (S x) n x impossible1)).
      case impossible2.
Qed.

(* The modulo operation *)
Fixpoint modulo_ex (a r n:nat) : nat :=
  match a with
    | O => match r with
      | O => O
      | S r_1 => n-r
    end
    | S a_1 => match r with
      | O => modulo_ex a_1 (n-1) n
      | S r_1 => modulo_ex a_1 r_1 n
    end
  end.
Definition modulo (a n:nat) : nat := modulo_ex a O n.

Lemma add_transitive : forall (a b c:nat), a+b+c = a+(b+c).
Proof.
  admit.
Qed.

Require Import Minus.

(* Proof that the modulo operation we defined is well-formed regarding to is_modulo *)
(* Note the fine-grained case for when n=0. *)
Theorem modulo_well_formed :
  forall (a n:nat), IF n=0 then (modulo a n)=0 else is_modulo a n (modulo a n).
Proof.
  unfold IF_then_else.
  destruct n.
    (* Case n=0 *)
    refine (or_introl _).
    refine (conj (eq_refl O) _).
    unfold modulo.
    elim a.
      (* Case a=0 *)
      unfold modulo_ex.
      reflexivity.
      (* Case a --> a+1 *)
      intros n H.
      unfold modulo_ex.
      assert (repl_zero : (0 - 1) = 0).
      simpl.
      reflexivity.
      exact H.
    (* Case n>0 *)
    refine (or_intror _).
    refine (conj _ _).
    intro Hfalse.
    discriminate Hfalse.
    unfold is_modulo, modulo, lt.
    assert (firstPart : S (modulo_ex a 0 (S n)) <= S n).
      (* Proof that the modulo result is lower than n: *)
      assert (generalization : forall k, (k<=n) -> (S (modulo_ex a k (S n)) <= S n)).
      elim a.
        (* Case a=0 *)
        intro k.
        case k.
          (* Case k=0 *)
          intros H.
          unfold modulo_ex.
          exact (le_translates O n (zero_min n)).
          (* Case k>0 *)
          intros n0 useless.
          unfold modulo_ex.
          refine (le_translates (S n - S n0) n _).
          elim n.
          simpl.
          exact (le_n O).
          intros n1 H.
          simpl in H.
          simpl.
          destruct n0.
          exact (le_n (S n1)).
          exact (le_S (n1-n0) n1 (sub_lowers n1 n0)).
        (* Case a --> a+1 *)
        intros n0 H1 k.
        case k.
          (* Case k=0 *)
          intro H2.
          unfold modulo_ex.
          assert (subg1 : S (modulo_ex n0 (S n - 1) (S n)) <= S n).
          assert (subg2 : (S n - 1) <= n).
          simpl.
          exact (sub_lowers n O).
          exact (H1 (S n - 1) subg2).
          exact subg1.
          (* Case k>0 *)
          intros n1 H2.
          pose (le_n1_n := le_S_n n1 n (le_S (S n1) n H2)).
          unfold modulo_ex.
          exact (H1 n1 le_n1_n).
      exact (generalization O (zero_min n)).
    refine (conj _ _).
    exact firstPart.
      (* existence of k *)
      assert (generalization : forall l, (l<=n) -> (
        exists (k:nat), a+S n = k*(S n) + l + modulo_ex a l (S n))).
      elim a.
        (* Case a=0 *)
        intros l.
        case l.
          (* Case l=0 *)
          intro useless.
          unfold modulo_ex.
          refine (ex_intro _ 1 _).
          rewrite (plus_comm (1 * S n + O) O).
          rewrite (plus_comm (1 * S n) O).
          unfold mult.
          rewrite (plus_comm (S n) O).
          unfold plus.
          reflexivity.
          (* Case l>0 *)
          intros n0 H.
          unfold modulo_ex.
          pose (simplif := (le_plus_minus (S n0) (S n) (le_S (S n0) n H))).
          refine (ex_intro _ O _).
          rewrite (add_transitive (0*S n) (S n0) (S n - S n0)).
          rewrite <- simplif.
          unfold mult, plus.
          reflexivity.
        (* Case a --> a+1 *)
        intros n0 H1 l.
        destruct l.
          (* Case l=0 *)
          intro H2.
          pose (H1red := H1 n (le_n n)).
          inversion H1red.
          refine (ex_intro _ (S x) _).
          unfold mult, modulo_ex, plus at 1, minus.
          assert (subg1 : S (n0 + S n) = S n + x * (S n) + 0 + modulo_ex n0 (n - O) (S n)).
          rewrite (sub_zero n).
          unfold plus at 4, plus at 3, plus at 2.
          assert (subg2 : n0 + S n = n + x * S n + 0 + modulo_ex n0 n (S n)).
          rewrite (plus_comm n (x * S n)).
          rewrite (plus_n_O (x * S n + n)).
          exact H.
          rewrite subg2.
          reflexivity.
          exact subg1.
          (* Case l>0 *)
          intro H2.
          pose (H1red := H1 l (le_S_n l n (le_S (S l) n H2))).
          inversion H1red.
          refine (ex_intro _ x _).
          unfold modulo_ex, plus at 1.
          assert (subg1 : S (n0 + S n) = x * S n + S l + modulo_ex n0 l (S n)).
          rewrite <- (plus_1_vs_S l).
          rewrite <- (plus_transitive (x*S n) l 1).
          rewrite (plus_transitive (x * S n + l) 1 (modulo_ex n0 l (S n))).
          rewrite (plus_comm 1 (modulo_ex n0 l (S n))).
          rewrite <- (plus_transitive (x*S n + l) (modulo_ex n0 l (S n)) 1).
          rewrite <- H.
          rewrite (plus_1_vs_S (n0 + S n)).
          reflexivity.
          exact subg1.
      (* todo use generalization *)
      pose (genred := generalization O (zero_min n)).
      inversion genred.
      destruct x.
        (* Case x=0 *)
        unfold mult, plus at 3, plus at 2 in H.
        rewrite <- H in firstPart.
        pose (wrong1 := le_S_n (a + S n) n firstPart).
        rewrite (plus_comm a (S n)) in wrong1.
        pose (wrong2 := le_transitive (S n) (S n + a) n (conj (plus_vs_le (S n) a) wrong1)).
        case (le_wrong n wrong2).
        (* Case x>0 *)
        refine (ex_intro _ x _).
        refine (plus_funny_proof a (S n) (x * S n + modulo_ex a 0 (S n)) _).
        rewrite (plus_n_O (S x * S n)) in H.
        rewrite <- (plus_transitive (S n) (x * S n) (modulo_ex a 0 (S n))).
        unfold mult in H.
        exact H.
Qed.
