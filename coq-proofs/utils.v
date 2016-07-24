(* Proof that (a <= 0) -> (a = 0) *)
Theorem le_zero :
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
Theorem le_transitive :
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
Theorem zero_min : forall (x:nat), 0 <= x.
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
Theorem le_translates : forall (a b:nat), (a <= b) -> ((S a) <= (S b)).
Proof.
  admit. (* TODO *)
Qed.

(* Proof that <= is a totally ordered relation (fine grained version) *)
Theorem le_tot_ordered : forall (a b:nat), (a<=b) \/ (a>b).
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
Theorem plus_n_O : (forall n, n + O = n).
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

(* Proof that + commutes *)
Theorem plus_comm : (forall (n m:nat), n + m = m + n).
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

Definition is_modulo (a n r:nat) : Prop :=
  (r<n) /\ (exists (k:nat), a=(k*n+r))
.

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
      admit. (* TODO finish *)
Qed.

(* TODO
Definition modulo (a n:nat) : nat :=
  match n with
    | O => a
    | S x => match (modulo_existence a (S x) (le_translates 0 x (zero_min x)): ex (fun r => is_modulo a n r)) with
      | ex_intro w proof_of_Pw => w
    end
  end
.

Theorem modulo_well_formed :
  forall (a n:nat), (n<>0) -> is_modulo a n (modulo a n).
Proof.

Qed.
*)